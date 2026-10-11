## 05. Segurança e robustez

[voltar ao índice](../14-external-esp32-communication.md)

---

### 1. Ponto de vista de segurança

Com a opção 2, o **controle físico** (servo, HX711, botões) vive no ESP32 externo.
A segurança do atuador **não pode depender da tela nem do link UART**.

Regras:

- o **botão de emergência** corta/para o dispenser **no externo**,
  independentemente do estado da UI/link;
- o externo é a única autoridade sobre `dispenser.start()/stop()`;
- a tela é apenas um **cliente de exibição/comando**, nunca o árbitro de segurança.

---

### 2. Emergência no externo (decisão)

A emergência (RP embarcado — parada física do servo) deve ser ligada ao
**ESP32 externo**, pois:

- é nele que o servo está fisicamente conectado;
- a parada por emergência não depende da telemetria estar atualizada;
- `dosing_controller_cancel()` já centraliza a parada do dispenser no domínio —
  o externo apenas o executa com entrada do botão, sem passar pela UI.

Fluxo:

```text
Botão EMERGENCIA (físico, externo)
   ↓
DosingController.cancel()  (no externo)
   ↓
Dispenser.stop()
   ↓
STATE=INTERRUPTED → enviado à tela (se link ativo)
```

---

### 3. Falha de comunicação

| Cenário | Comportamento seguro |
| ------- | -------------------- |
| Link cai em `DOSING` | Externo **mantém/continua** dosagem? Não: define-se parada segura. Recomendado: continuar a dosagem localmente (o externo é autônomo) e registrar; ou parar e entrar em `INTERRUPTED`. Decisão de produto (ver "definições abertas"). |
| Link cai em outro estado | Externo permanece no estado local; tela mostra "SEM COMUNICACAO". |
| Tela reinicia | Externo envia estado atual; tela se re-sincroniza pelo handshake. |
| Externo reinicia | Reconstrói config salva e aguarda `PING`/`CONFIG_SET`.

O domínio já preserva o peso parcial e só zera com `new_dosing()`, o que ajuda a
manter consistência entre reinícios — ver
[04-domain-and-state-machine.md](../04-domain-and-state-machine.md).

---

### 4. Config persistida no externo

A configuração (modo, gramas/valor, preço) é **fonte de verdade no externo**:

- é enviada pela tela em `CONFIG_SET`;
- o externo pode persistir em NVS/EEPROM para sobreviver a reinícios;
- a tela pede o estado atual no handshake e espelha sem regra de negócio
  (a vida segue em [07-ui-and-navigation.md](../07-ui-and-navigation.md)).

---

### 5. Checklist de segurança do link

- [x] emergência sempre acionável no externo mesmo com tela desligada;
- [x] timeout de link não deixa servo em estado inseguro (o externo segue
  autônomo; a tela apenas indica "SEM COMUNICACAO");
- [x] CRC/desync não gera comandos falsos (frame descartado; re-sync no `0xAA`);
- [x] telemetria não altera estado do domínio (somente exibição);
- [x] `CANCEL` originado na UI chega ao externo e para o dispenser.

### 6. Estado da implementação

No firmware atual, o externo persiste a config em **NVS** (`Preferences`,
namespace `dosing`) e sinaliza falha do HX711 via `ERROR`; a tela exibe um
**overlay** com "SEM COMUNICACAO" (link caído) ou "ERRO DO SENSOR DE PESO", e
regera `PING`/`CONFIG_SET` até obter resposta. Falta apenas a validação física
em bancada (pinos e ruído).

---

[voltar ao índice](../14-external-esp32-communication.md)