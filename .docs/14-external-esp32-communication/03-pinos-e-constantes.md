## 03. Pinos e constantes

[voltar ao índice](../14-external-esp32-communication.md)

---

### 1. Princípio

Os pinos do UART devem ser definidos por **constantes** (no padrão
`board_config.h` do projeto) e **validados contra a montagem física** antes do
primeiro firmware — a mesma regra de pinagem do restante do hardware, ver
[08-target-hardware/11-limitacoes-e-seguranca.md](../08-target-hardware/11-limitacoes-e-seguranca.md).

Os valores abaixo são uma **proposta**; ajuste as constantes conforme a placa
externa e a disponibilidade real dos GPIOs.

---

### 2. UART escolhido

Requisitos:

- **Taxa**: `115200` baud, bidirecional (TX/RX cruzados).
- **Não usar UART0** (USB serial) — reservado para debug/flash no DevKit e na
  placa VIEWE.
- Preferir **UART2** (em ambas as placas) para deixar UART0/UART1 livres.

Conexão física:

```text
ESP32-S3 (tela)  TX ────────── RX  ESP32 externo
ESP32-S3 (tela)  RX ────────── TX  ESP32 externo
GND (comum)      ────────────────  GND
```

---

### 3. Constantes propostas — placa da tela (`board_config.h`)

```c
/* ⋯ (prefixo do restante do board_config.h) ⋯ */

#define BOARD_LINK_UART_NUM 2
#define BOARD_LINK_TX_PIN   12
#define BOARD_LINK_RX_PIN   13
#define BOARD_LINK_BAUD     115200
```

Justificativa da escolha na placa VIEWE (N16R8):

- Os pinos **10–13** ficam livres quando a opção 2 move HX711/servo/botões para
  o externo (hoje são o slot microSD, conforme
  [08-target-hardware/11-limitacoes-e-seguranca.md](../08-target-hardware/11-limitacoes-e-seguranca.md)).
- **12 (TX) e 13 (RX)** não conflitam com display/touch/backlight já usados
  (39, 41, 40, 42 + D0–D15 + 19/20 + 38 + 2).
- Evitar **18** (INT do touch) e **17** (WS2812B) para não competir com trilhas
  do módulo; **33–37** ficam reservados à PSRAM octal no N16R8 e não devem ser
  usados; **26–32** são do flash/PSRAM.

---

### 4. Constantes propostas — ESP32 externo (novo `board_config_ctrl.h`)

O controlador externo usa pinos livres do próprio DevKit/module; a escolha abaixo
é genérica e deve ser conferida na placa real. Note que os **nomes das macros são
os mesmos** do `board_config.h` da tela (`BOARD_LINK_*`): assim o mesmo
`link_uart_esp32.cpp` compila nas duas pontas, escolhendo o arquivo de config por
`#if defined(ROLE_EXTERNAL)`.

```c
#define BOARD_LINK_UART_NUM 2
#define BOARD_LINK_TX_PIN   17
#define BOARD_LINK_RX_PIN   16
#define BOARD_LINK_BAUD     115200
```

---

### 5. Validação pendente (física)

Antes de fixar o valor:

- [ ] conferir se **12/13** estão quebrados para fora na placa VIEWE;
- [ ] confirmar que **17/16** do externo não colidem com flash/PSRAM/boot;
- [ ] testar o link com **loopback** (TX ligado no RX da mesma placa);
- [ ] medir nível lógico e GND comum entre as duas placas.

---

[voltar ao índice](../14-external-esp32-communication.md)