## 01. Decisão e visão geral

[voltar ao índice](../14-external-esp32-communication.md)

---

### 1. Contexto

O projeto foi desenvolvido com o ESP32-S3 integrado à tela como **único**
microcontrolador (opção 1): ele roda LVGL, domínio, HX711, servo e botões.

Durante a análise do hardware físico, surgiu a possibilidade de usar um
**segundo ESP32** (externo) que concentra todo o controle de atuadores e
sensores, deixando o ESP32-S3 da tela apenas com a responsabilidade de
**renderizar a UI** e capturar o toque.

Esta organização documenta essa arquitetura, os motivos e o que ela muda.

---

### 2. Comparação entre as opções

| Aspecto              | Opção 1: ESP32-S3 integrado      | Opção 2: ESP32 externo + ESP32 da tela |
| -------------------- | -------------------------------- | -------------------------------------- |
| Número de MCUs       | 1                                | 2                                      |
| Onde roda o domínio  | ESP32-S3 integrado               | ESP32 externo                          |
| Onde roda a UI/LVGL  | ESP32-S3 integrado               | ESP32-S3 integrado (só render)         |
| HX711 / servo / botões | ESP32-S3 integrado             | ESP32 externo                          |
| Comunicação entre MCUs | —                              | UART (bidirecional)                    |
| Fios/complexidade    | Menor                            | Maior (cabo UART + GND comum)          |
| Custo                | Menor                            | Maior (2º ESP32)                       |
| Liberdade de GPIOs no externo | —                            | Alta (placa dedicada só p/ controle)   |
| Dependência da tela  | Total                            | UI fica funcionalmente separada        |

---

### 3. Decisão adotada

A **opção 2 (ESP32 externo + ESP32 da tela)** é a **adotada** para o firmware
real: a pinagem da tela ficou apertada demais (display, touch, microSD, UART,
BOOT) e a separação deixa o controle físico independente da UI. A **opção 1
(integrada)** foi **descontinuada**.

| Item | Decisão |
| ---- | ------- |
| Transporte | **UART** |
| Config da dosagem | **persistida no ESP32 externo** |
| Botão de emergência | **no ESP32 externo** |
| Simulador PC | **inalterado** |

---

### 4. Visão de comunicação

```text
ESP32-S3 (tela)                    ESP32 externo
┌─────────────────────┐           ┌──────────────────────┐
│ LVGL (UI)           │           │ DosingController     │
│ Touch (input)       │◄────────►│ WeightSensor (HX711) │
│ estado espelhado    │   UART    │ Dispenser (servo)    │
│ da tela             │           │ Botões / LED         │
└─────────────────────┘           └──────────────────────┘
```

- **Tela → externo**: comandos do usuário (iniciar, cancelar, configurar,
  liberação manual, nova dosagem).
- **Externo → tela**: telemetria (peso atual, estado, fase, meta, percentual).

O domínio continua sendo a única autoridade sobre o estado da dosagem.

---

[voltar ao índice](../14-external-esp32-communication.md)