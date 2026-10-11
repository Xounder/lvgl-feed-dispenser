## 07. Conexões físicas e pinagem

[voltar ao índice](../14-external-esp32-communication.md)

---

### 1. Visão geral

O sistema físico tem **duas placas** que se falam por **UART**:

```text
ESP32-S3 da tela                 ESP32 externo
(LVGL + touch + proxy UART)      (domínio + HX711 + servo + botões/LED)
        GPIO12 (TX) ───────────► GPIO16 (RX)
        GPIO13 (RX) ◄─────────── GPIO17 (TX)
        GND         ───────────── GND        (referência comum)
```

São apenas **3 fios** entre as placas: **TX→RX**, **RX→TX** e **GND**. Nenhum
alimentação é trocada entre elas (cada placa usa o **seu** USB). Isso é o
"**TX↔RX cruzado + GND comum**" citado no README.

---

### 2. Por que "cruzado" e por que GND comum

- **TX↔RX (cruzado)**: o `TX` (transmissor) de uma placa precisa chegar no `RX`
  (receptor) da outra. Ligar TX→TX não funciona; é sempre **saída → entrada**:
  - `Tela.TX (GPIO12)` → `Externo.RX (GPIO16)`
  - `Tela.RX (GPIO13)` → `Externo.TX (GPIO17)`
- **GND comum**: as duas placas só "entendem" o mesmo nível lógico se
  compartilharem a **mesma referência** (0 V). Sem GND comum o link lê lixo.
- **Não ligar 5V/3V3 entre placas**: cada uma já é alimentada pelo seu USB; ligar
  as alimentações juntas pode causar conflito. Compartilhe **só o GND**.
- **Nível lógico 3,3 V**: as duas são ESP32-S3, então os níveis já são
  compatíveis — não precisa de conversor de nível.

---

### 3. Pinagem do link (UART)

Ambas as placas usam **UART2** e `115200` baud (constante `BOARD_LINK_*`).
Os nomes das macros são iguais nas duas pontas; o valor difere por placa:

| Placa | Arquivo | `BOARD_LINK_TX_PIN` | `BOARD_LINK_RX_PIN` | UART | Baud |
| ----- | ------- | ------------------- | ------------------- | ---- | ---- |
| Tela (ESP32-S3 touch) | `src/hardware/esp32/board_config.h` | **12** | **13** | 2 | 115200 |
| Externo | `src/hardware/esp32/board_config_ctrl.h` | **17** | **16** | 2 | 115200 |

Ligação resultante (cabo de 3 vias):

| Sinal | Pino na tela | Pino no externo |
| ----- | ------------ | --------------- |
| Dados tela→externo | GPIO12 (TX) | GPIO16 (RX) |
| Dados externo→tela | GPIO13 (RX) | GPIO17 (TX) |
| Referência | GND | GND |

> Os números são os valores atuais em `board_config.h` / `board_config_ctrl.h`.
> Se mudar lá, a tabela acima muda junto (fonte única). Validação física
> pendente em [03-pinos-e-constantes.md](03-pinos-e-constantes.md) §5.

---

### 3.1 Cuidados na placa da tela (VIEWE)

- GPIO12/13 pertencem ao grupo do slot microSD da VIEWE: use **sem cartão no
  slot** e o firmware **não** inicializa o SPI do SD.
- Evite usar os pinos **reservados** do módulo: **26–32** (flash/PSRAM) e
  **33–37** (PSRAM octal no N16R8).

---

### 4. Periféricos do ESP32 externo

O externo concentra todo o hardware de controle. Pinos em `board_config_ctrl.h`:

| Periférico | Pino (GPIO) | Observação |
| ---------- | ----------- | ---------- |
| HX711 — DT (DATA) | 4 | célula de carga → módulo HX711 |
| HX711 SCK | 5 | clock do HX711 |
| Servo (PWM) | 6 | servo do dosador; **alimentar por fonte própria** |
| Botão de liberação manual (1) | 7 | para GND (usa `INPUT_PULLUP`) |
| Botão de emergência (2) | 15 | para GND (pull-up interno) |
| LED indicador | 8 | LED simples (ativo alto) |

Observações elétricas:

- **Botões**: ligados entre o GPIO e o **GND**, com `INPUT_PULLUP` do próprio
  ESP32 (pressionado = nível baixo). Sem resistor externo é aceitável para
  bancada; para robustez use pull-up externo de 10 kΩ.
- **Servo**: alimente com **fonte própria** (a corrente do servo não sai do
  regulador do DevKit); o GND da fonte do servo **precisa** ser comum ao GND do
  ESP32. O pino de sinal é o GPIO6.
- **HX711**: além de DT/SCK, ligue `VCC`/`GND` do módulo na fonte correta
  (3,3 V) e a **célula de carga** no HX711.
- **LED**: se quiser, use um resistor em série (ex.: 220 Ω–1 kΩ) para limitar a
  corrente do GPIO.

---

### 4.1 Diagramas de ligação (ASCII)

#### Link UART entre as placas (3 fios)

```text
   Tela (ESP32-S3)                          Externo (ESP32)
   ┌───────────────┐                        ┌───────────────┐
   │ GPIO12  TX    ├───────────────────────►│ RX    GPIO16  │
   │ GPIO13  RX    │◄───────────────────────┤ TX    GPIO17  │
   │ GND           ├────────────────────────┤ GND           │
   └───────────────┘                        └───────────────┘
        (não trocar 3V3/5V entre as placas — só o GND)
```

#### HX711 + célula de carga

```text
   Externo                  Módulo HX711              Célula de carga
   ┌───────────┐            ┌───────────┐            ┌───────────┐
   │ 3V3       ├───────────►│ VCC       │            │           │
   │ GND       ├───────────►│ GND       │            │  barra    │
   │ GPIO4 DT  │◄───────────┤ DT        │            │  (célula) │
   │ GPIO5 SCK ├───────────►│ SCK       │            │           │
   └───────────┘            │ E+  E-  A+  A- ───────►│ E+ E- A+ A-│
                            └───────────┘            └───────────┘
     E+/E- = excitação (alimentação da célula)   A+/A- = sinal (leitura)
```

#### Servo (fonte externa + GND comum)

```text
   Fonte do servo (≈5V)        Servo                      Externo
   ┌───────────┐            ┌───────────┐
   │ +5V       ├───────────►│ VCC (verm.)│
   │ GND ──────┼───┬───────►│ GND (mar.) │
   └───────────┘   │        │           │
                   │        │ Sinal(lar.)├─────────────►│ GPIO6
                   └────────────────────────────────────►│ GND (comum)
     (o GND da fonte do servo PRECISA ser o mesmo do ESP32)
```

#### Botões (INPUT_PULLUP — solto = 1, apertado = 0)

```text
   Externo GPIO7  ────┬──── [ Botão liberação ] ──── GND
                      └── pull-up interno (INPUT_PULLUP)

   Externo GPIO15 ────┬──── [ Botão emergência ] ─── GND
                      └── pull-up interno (INPUT_PULLUP)
```

#### LED indicador (com resistor em série)

```text
   Externo GPIO8 ────[ R ≈ 220Ω–1kΩ ]────►|──── GND
                                          LED
                                    (ânodo → cátodo)
```

---

### 5. Ordem de ligação e teste

1. Desligue as duas placas (sem USB).
2. Ligue os 3 fios do link (TX↔RX, RX↔TX, GND).
3. Ligue os periféricos no **externo** (HX711, servo, botões, LED).
4. Conecte os **dois** USB (a tela continua no USB da tela; o externo no USB
   dele). O GND do USB já é o GND comum, mas **mantenha** o fio de GND dedicado.
5. Grave primeiro o **externo** (`arduino_ctrl/`) e depois a **tela**
   (`arduino/`) — ver [README global](../../README.md#modo-arduino-esp32-s3).
6. Na tela, o overlay deve sumir quando o link subir (sem "SEM COMUNICACAO").

> **Teste de loopback (opcional):** antes de ligar as duas, feche TX no RX da
> **mesma** placa e confirme o eco no monitor serial a 115200. Ajuda a isolar
> problemas de fio/solda.

---

### 6. Relacionados

- [03-pinos-e-constantes.md](03-pinos-e-constantes.md) — valores e validação dos pinos.
- [04-protocolo-e-mensagens.md](04-protocolo-e-mensagens.md) — o que trafega no UART.
- [05-seguranca-e-robustez.md](05-seguranca-e-robustez.md) — link caído / emergência.
- [`arduino/README.md`](../../arduino/README.md) — passo a passo dos dois sketches.
- [08-target-hardware/11-limitacoes-e-seguranca.md](../08-target-hardware/11-limitacoes-e-seguranca.md) — restrições elétricas.

---

[voltar ao índice](../14-external-esp32-communication.md)
