# Comunicação ESP32 Externo → ESP32 da Touchscreen

> **Documento canônico** da arquitetura física **adotada** para o firmware real:
> um **ESP32 externo** concentra todo o controle (domínio, HX711, servo, botões,
> LED) e o **ESP32-S3 da tela** fica como **cliente de renderização** (LVGL +
> touch), recebendo estado e enviando comandos pelo barramento UART.
> Este arquivo é o índice; o conteúdo completo está nas partes listadas abaixo.

---

## Resumo

O ESP32-S3 integrado à tela (opção 1) rodava **tudo**: UI, domínio, HX711, servo
e botões. A arquitetura **adotada** (opção 2) divide o sistema em **dois
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

Decisões adotadas:

- **Transporte**: UART (recomendado — simples, determinístico, sem stack de rede).
- **Config da dosagem**: persistida no **ESP32 externo** (fonte da verdade);
  a tela apenas envia os valores digitados.
- **Botão de emergência**: conectado ao **ESP32 externo** (quem controla o
  atuador), independente do estado do link/display.
- **Simulador PC**: **não muda** — continua rodando domínio + hardware simulado
  localmente; a opção 2 é o firmware real.

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