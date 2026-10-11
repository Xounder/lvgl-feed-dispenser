# Comunicação ESP32 Externo → ESP32 da Touchscreen

> **Documento canônico** sobre a evolução da arquitetura física: tornar opcional o
> uso de um **ESP32 externo** que concentra todo o controle (domínio, HX711,
> servo, botões, LED) enquanto o **ESP32-S3 da tela** passa a ser apenas um
> **cliente de renderização** (LVGL + touch), recebendo os comandos de atualização
> de tela pelo barramento de comunicação.
> Este arquivo é o índice; o conteúdo completo está nas partes listadas abaixo.

---

## Resumo

Hoje (opção 1) o ESP32-S3 integrado à tela roda **tudo**: UI, domínio, HX711,
servo e botões. A alternativa estudada (opção 2) divide o sistema em **dois
microcontroladores**:

```text
                    ┌───────────────────────────┐
                    │     ESP32-S3 (tela)       │
                    │  LVGL + Touch (UI)        │
                    └───────────┬───────────────┘
                                │  UART
                                │
                    ┌───────────┴───────────────┐
                    │      ESP32 externo        │
                    │  DosingController (domínio)│
                    │   HX711 · Servo · Botões  │
                    └───────────────────────────┘
```

Decisões já tomadas nesta análise:

- **Transporte**: UART (recomendado — simples, determinístico, sem stack de rede).
- **Config da dosagem**: persistida no **ESP32 externo** (fonte da verdade);
  a tela apenas envia os valores digitados.
- **Botão de emergência**: conectado ao **ESP32 externo** (quem controla o
  atuador), independente do estado do link/display.
- **Simulador PC**: **não muda** — continua rodando domínio + hardware simulado
  localmente; a opção 2 é adicionada como alternativa do firmware real.

---

## Índice

| Parte | Conteúdo |
| ----- | -------- |
| [01-decisao-e-visao-geral.md](14-external-esp32-communication/01-decisao-e-visao-geral.md) | Contexto, comparação opção 1 × opção 2, decisão adotada e motivos |
| [02-arquitetura-e-camadas.md](14-external-esp32-communication/02-arquitetura-e-camadas.md) | O que fica na tela, o que fica no externo, fronteira de dependência e impacto no código atual |
| [03-pinos-e-constantes.md](14-external-esp32-communication/03-pinos-e-constantes.md) | Pinos sugeridos para o UART (constantes editáveis no estilo `board_config.h`) e validações pendentes |
| [04-protocolo-e-mensagens.md](14-external-esp32-communication/04-protocolo-e-mensagens.md) | Protocolo UART: fluxo de mensagens (comandos ↔ telemetria), handshake, timeout e tratamento de erro |
| [05-seguranca-e-robustez.md](14-external-esp32-communication/05-seguranca-e-robustez.md) | Emergência no externo, falha de comunicação e comportamento seguro |
| [06-plano-de-implementacao.md](14-external-esp32-communication/06-plano-de-implementacao.md) | Etapas propostas, critérios para validar cada uma e riscos |
| [07-conexoes-fisicas.md](14-external-esp32-communication/07-conexoes-fisicas.md) | Ligações físicas entre as duas placas (TX↔RX + GND) e pinagem dos periféricos do externo |

---

## Documentos relacionados

- [08-target-hardware.md](08-target-hardware.md) — hardware alvo (display, HX711, servo, botões, LED)
- [03-architecture-decisions/07-esp32-integrado-vs-externo.md](03-architecture-decisions/07-esp32-integrado-vs-externo.md) — decisão (ADR) entre ESP32 integrado e externo
- [05-hardware-abstraction.md](05-hardware-abstraction.md) — abstrações `WeightSensor`/`Dispenser`
- [04-domain-and-state-machine.md](04-domain-and-state-machine.md) — domínio e máquina de estados
- [11-migration-pc-to-esp32.md](11-migration-pc-to-esp32.md) — migração PC → ESP32-S3
- [12-roadmap.md](12-roadmap.md) — etapas planejadas