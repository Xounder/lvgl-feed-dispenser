## 02. Arquitetura e camadas

[voltar ao índice](../14-external-esp32-communication.md)

---

### 1. Separação do que roda onde

| Camada                | ESP32-S3 da tela | ESP32 externo |
| --------------------- | ---------------- | ------------- |
| UI (LVGL + telas)     | sim              | não           |
| Touch input           | sim              | não           |
| Domínio (DosingController) | não (apenas estado espelhado) | sim |
| WeightSensor (HX711)  | não              | sim           |
| Dispenser (servo)     | não              | sim           |
| Botões físicos / LED  | não              | sim           |
| Protocolo de comunicação | cliente (proxy) | servidor (backend) |

---

### 2. Fronteira de dependência

A regra das camadas (ver [05-hardware-abstraction.md](../05-hardware-abstraction.md))
continua valendo:

```text
UI pode conhecer o domínio (por API/funções)
UI NÃO conhece HX711, servo, GPIO, PWM
Domínio NÃO conhece LVGL, SDL2, protocolo
```

Na opção 2, a UI não chama o `DosingController` diretamente: ela chama um
**proxy** que conversa via UART com o controlador externo. Isso exige manter:

- os mesmos nomes de funções/semântica (para a UI não mudar);
- o domínio puro rodando no externo (sem LVGL).

---

### 3. Impacto no código atual

#### O que NÃO muda

- `src/domain/` — domínio puro, reutilizado no ESP32 externo.
- `src/ui/` — telas LVGL, reutilizadas na tela (com o proxy por trás).
- `src_pc/` — simulador PC: continua chamando o domínio localmente.
- Interfaces `weight_sensor.h` / `dispenser.h` — continuam sendo o contrato.

#### O que muda

- o antigo entry point único (`main_esp32.cpp`) é **substituído** por dois:
  - `main_ui_esp32.cpp` → setup/loop da tela (LVGL + proxy UART);
  - `main_ctrl_esp32.cpp` → setup/loop do externo (domínio + hardware real).
- Nova camada de **protocolo** (`src/link/`):
  - serialização/recebimento das mensagens UART;
  - proxy do lado da tela;
  - backend do lado do externo.

---

### 4. Modelo de build: um sketch por firmware (Arduino IDE)

O Arduino IDE compila **toda** a árvore do sketch (inclusive a junction `src/`),
portanto o papel **não** pode ser decidido em runtime: precisa ser resolvido em
tempo de compilação, **um sketch por firmware**. Não há build único que "junta"
tudo — os firmwares são independentes e se falam por UART.

```text
arduino/        (tela)     -> junction src/ ; build_opt.h: -DROLE_SCREEN
arduino_ctrl/   (externo)  -> junction src/ ; build_opt.h: -DROLE_EXTERNAL
```

- `arduino/` é o **ESP32-S3 da tela** (LVGL + proxy UART); não precisa de
  HX711/servo.
- `arduino_ctrl/` é o **ESP32 externo** (domínio + HX711 + servo + botões/LED);
  não precisa de LVGL.
- a opção integrada (firmware único no board tela touch) **não existe mais**:
  restam apenas os dois firmwares acima e o simulador PC.
- `board_config.h` guarda só o que a **tela** usa (display/touch/backlight +
  `BOARD_LINK_*`); HX711/servo/botões/LED ficam em `board_config_ctrl.h`.
- `create_sketch_links.ps1` cria a junction `src/` dos **dois** sketches.

#### Como evitar símbolos duplicados

Como todos os sketches enxergam o mesmo `src/` por junction, os arquivos de
**papel único** levam uma guarda de compilação no topo (os `#include` de
hardware/UI entram **dentro** da guarda):

| Arquivo | Guarda | Ativo em |
| ------- | ------ | -------- |
| `src/main_ui_esp32.cpp` | `#ifdef ROLE_SCREEN` | tela |
| `src/main_ctrl_esp32.cpp` | `#ifdef ROLE_EXTERNAL` | externo |
| `src/domain/dosing_controller.c` | `#ifndef ROLE_SCREEN` | externo |
| `src/link/link_proxy.cpp` | `#ifdef ROLE_SCREEN` | tela |
| `src/link/link_backend.cpp` | `#ifdef ROLE_EXTERNAL` | externo |
| `src/link/link_uart_esp32.cpp` | `#if defined(ROLE_SCREEN) || defined(ROLE_EXTERNAL)` | tela, externo |
| `src/hardware/esp32/real_*.cpp` | `#ifndef ROLE_SCREEN` | externo |
| `src/hardware/esp32/physical_controls.cpp` | `#ifndef ROLE_SCREEN` | externo |
| `src/hardware/esp32/board_display.cpp` | `#ifndef ROLE_EXTERNAL` | tela |
| `src/ui/**/*.c` | `#ifndef ROLE_EXTERNAL` | tela |

Assim, a tela obtém os `dosing_controller_*` do **proxy** (o domínio real vira
unidade de tradução vazia) e o externo obtém do domínio real (o proxy fica
vazio). A UI e o driver de display ficam fora do firmware externo.

---

### 4.1 Estrutura

```text
arduino/                    # sketch da tela (build_opt.h: -DROLE_SCREEN)
arduino_ctrl/               # sketch do externo (build_opt.h: -DROLE_EXTERNAL)
                            # cada um tem junction `src/` -> ../src

src/
├── domain/                 # puro, roda no externo (dosing_controller.c guardado)
├── ui/                     # telas LVGL, roda na tela (guardado #ifndef ROLE_EXTERNAL)
├── hardware/
│   ├── weight_sensor.h     # contrato
│   ├── dispenser.h         # contrato
│   └── esp32/              # reais + board_display.cpp (guardados por papel)
├── link/                   # protocolo UART
│   ├── link_config.h       # IDs de mensagem + parâmetros
│   ├── link_uart.h         # transporte (vtable)
│   ├── link_uart_esp32.cpp # transporte no ESP32 (UART2)
│   ├── link_frame.c/h      # frame + CRC8
│   ├── link_proxy.cpp      # proxy p/ UI (tela)
│   └── link_backend.cpp    # backend p/ domínio (externo)
├── main_ui_esp32.cpp       # entry point da tela
└── main_ctrl_esp32.cpp     # entry point do externo
```

O `CMakeLists.txt` do PC continua compilando apenas o que o simulador usa —
as novas entidades do link ficam fora do build do PC, preservando o simulador
intacto.

---

[voltar ao índice](../14-external-esp32-communication.md)