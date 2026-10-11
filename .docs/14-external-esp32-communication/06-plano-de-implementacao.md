## 06. Plano de implementação

[voltar ao índice](../14-external-esp32-communication.md)

---

### 1. Fase 0 — validação do link (já planejada)

Sem alterar o código de estúdio da UI/domínio, validar o **transporte** primeiro:

- criar os dois sketches (`arduino/` tela e `arduino_ctrl/` externo), cada um
  com junction de `src/` e `build_opt.h` definindo o papel (ver
  [02-arquitetura-e-camadas.md](02-arquitetura-e-camadas.md), seção de build);
- placas conectadas (TX↔RX cruzado + GND comum);
- loopback em cada placa;
- `PING`/`PONG` entre as duas;
- metragem do cabo e ruído em 115200 baud.

Critério de aceite: `PING/PONG` estável por 5 min sem perda e CRC íntegro.

---

### 2. Fase 1 — protocolo em firmware de teste

- implementar `src/link/` (`link_uart.h`, `link_frame.c/h`);
- dois firmwares mínimos de teste (eco/`PING`/`STATE`), um por sketch;
- aplicar as guardas de papel (`ROLE_SCREEN`/`ROLE_EXTERNAL`) nos arquivos
  listados na seção de build;
- validar `CONFIG_SET`/`ACK` e telemetria `WEIGHT`/`TARGET`.

Não usar ainda a UI real.

---

### 3. Fase 2 — proxy na tela e backend no externo

- **tela**: `link_proxy` implementando **os mesmos símbolos** de
  `dosing_controller.h` (`init`, `start`, `update`, `cancel`, `new_dosing`,
  `manual_release_*`, `get_weight/target_grams/state/phase`) — as telas não
  mudam;
- o proxy é bombeado pelo timer de UI (300 ms / 200 ms) **e** no `loop()` do
  `main_ui_esp32.cpp` (`link_proxy_poll()`), pois a Home não tem timer;
- **externo**: `link_backend` chama `dosing_controller_init(&cfg)` e o domínio
  real + interfaces reais (`board_config_ctrl.h`);
- handshake no boot e telemetria periódica.

Critério: todos os fluxos das telas funcionam igual ao simulador, mas com o
domínio rodando no externo.

---

### 4. Fase 3 — persistência e segurança

- persistir config em NVS no externo (namespace `dosing`, `Preferences`);
- botão **emergência** e **liberação manual** físicos no externo
  (`board_config_ctrl.h` define os pinos HX711/servo/botões/LED do externo; a
  tela **não** compila esses drivers);
- timeout/link-down tratado: a tela mostra **"SEM COMUNICACAO"** em um overlay e
  os comandos que mudam estado (`START`/`NEW_DOSING`/liberação manual) ficam
  bloqueados enquanto o link cai; `CANCEL` continua permitido; `PING`+`CONFIG_SET`
  são reenviados a cada `LINK_RETRY_MS` até a resposta do externo;
- `ERROR`: o externo sinaliza falha (ex.: HX711 ausente) e a tela exibe
  "ERRO DO SENSOR DE PESO";
- reinícios: a config sobrevive no externo; a tela resincroniza via handshake.

---

### 5. Fase 4 — integração e validação final

- fluxo completo Físico: configurar → dosar → concluir/interromper → nova dosagem;
- teste de interrupção por UI e por botão físico;
- reinício da tela e do externo em cada estado;
- documentar pinos definitivos e atualizar docs afetadas.

---

### 6. Ordem sugerida (dependências)

```text
link transporte (Fase 0)
   ↓
serialização + protocolo (Fase 1)
   ↓
proxy + backend (Fase 2)
   ↓
persistência + segurança (Fase 3)
   ↓
integração final (Fase 4)
```

Cada fase é validável no hardware antes de avançar; o simulador PC continua
sendo o contrato comportamental e não deve parar de compilar.

---

### 7. Riscos e mitigações

| Risco | Mitigação |
| ----- | --------- |
| Ruído/noite no UART em velocidade maior | iniciar em 115200; CRC8; re-sync no `0xAA` |
| Pinos 10–13 conflitarem na placa VIEWE | documentar e validar fisicamente na Fase 0 |
| Latência da telemetria | janela de 200–300 ms compatível com o timer de UI |
| Dupla fonte de verdade de config | config vive no externo; tela só espelha |
| Emergência depender da tela | botão físico no externo sempre priorizado |

---

[voltar ao índice](../14-external-esp32-communication.md)