## 07. ESP32-S3 integrado vs. ESP32 externo + tela

[voltar ao índice](../03-architecture-decisions.md)

---

### 1. Contexto

O projeto nasceu com o **ESP32-S3 integrado à tela** como único
microcontrolador (opção 1): ele roda a UI (LVGL), o domínio, o HX711, o servo
e os botões.

Ao levantar a pinagem da placa VIEWE, ficou claro que os GPIOs ficam
apertados: os periféricos de domínio sacrificam o slot microSD (GPIO10–13) e
ainda disputam o WS2812B (GPIO17) e o INT do GT911 (GPIO18). Surgiu então a
alternativa de um **segundo ESP32** (externo) que concentra o controle físico
e deixa a tela apenas renderizando a UI (opção 2).

O detalhamento completo dessa alternativa está em
[`14-external-esp32-communication.md`](../14-external-esp32-communication.md).

---

### 2. Decisão

- A **opção 2 (ESP32 externo + ESP32 da tela)** é a **adotada** para o firmware
  físico: a tela faz apenas UI (LVGL + touch) e um **proxy UART**; o controle
  (domínio, HX711, servo, botões/LED) fica no **ESP32 externo**.
- A **opção 1 (integrada)** — firmware único no ESP32-S3 da tela — foi
  **descontinuada e removida** do código nesta branch.
- O **simulador PC permanece inalterado**: continua rodando domínio +
  hardware simulado localmente, e segue como contrato comportamental.

Status: **Adotada (opção 2)**.

---

### 3. Motivos

| Critério               | Opção 1 (integrada)    | Opção 2 (externa)           |
| ---------------------- | ---------------------- | --------------------------- |
| Simplicidade           | menor (1 MCU)          | maior (2 MCUs + cabo UART)  |
| Custo                  | menor                  | maior (2º ESP32)            |
| Liberdade de GPIOs     | restrita               | alta no externo             |
| Isolamento de falhas   | UI e controle juntos   | UI separada do controle     |
| Dependência do link    | não há                 | tela depende do UART p/ estado |

---

### 4. Consequências

- A UI não muda: as abstrações `WeightSensor`/`Dispenser`
  ([`05-hardware-abstraction.md`](../05-hardware-abstraction.md)) permanecem o
  contrato; no firmware da tela elas são substituídas por um **proxy** que
  conversa por UART.
- A configuração da dosagem passa a ser **fonte de verdade no externo**.
- A **emergência** deve ficar no externo, independente da tela/link.
- Passam a existir um **protocolo** e tratamento de falha de link — ver
  [`14-external-esp32-communication/04-protocolo-e-mensagens.md`](../14-external-esp32-communication/04-protocolo-e-mensagens.md)
  e
  [`14-external-esp32-communication/05-seguranca-e-robustez.md`](../14-external-esp32-communication/05-seguranca-e-robustez.md).

---

### 5. Princípio

> **A tela renderiza, o externo controla.** O simulador PC permanece como
> contrato comportamental, sem mudanças.

---

[voltar ao índice](../03-architecture-decisions.md)
