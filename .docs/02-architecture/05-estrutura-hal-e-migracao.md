## 05. Estrutura `src/`, `main.c`, HAL e fronteira PC → ESP32

[voltar ao índice](../02-architecture.md)

---

### 1. Estrutura atual

A estrutura relevante do projeto é:

```text
src/
├── main.c
│
├── hal/
│   ├── hal.c
│   └── hal.h
│
├── ui/
│   ├── ui.c
│   ├── ui.h
│   ├── screen_manager.c
│   ├── screen_manager.h
│   └── screens/
│       ├── screen_chrome.c
│       ├── screen_chrome.h
│       ├── home_screen.c
│       ├── home_screen.h
│       ├── config_screen.c
│       ├── config_screen.h
│       ├── dosing_screen.c
│       ├── dosing_screen.h
│       ├── completed_screen.c
│       ├── completed_screen.h
│       ├── interrupted_screen.c
│       ├── interrupted_screen.h
│       ├── manual_release_widget.c
│       └── manual_release_widget.h
│
├── domain/
│   ├── dosing_config.h
│   ├── dosing_controller.c
│   └── dosing_controller.h
│
└── hardware/
    ├── weight_sensor.h
    ├── dispenser.h
    ├── simulated/
    │   ├── simulated_weight_sensor.c
    │   └── simulated_dispenser.c
    └── esp32/
        ├── board_config.h
        ├── board_display.h
        ├── board_display.cpp
        ├── real_weight_sensor.h
        ├── real_weight_sensor.cpp
        ├── real_dispenser.h
        └── real_dispenser.cpp
```

A organização dos arquivos de UI é detalhada em [07-ui-and-navigation.md](../07-ui-and-navigation.md).

---

### 2. `main.c` e HAL

O `main.c` é responsável principalmente pela inicialização da aplicação e pelo loop principal.

Conceitualmente:

```text
main
 │
 ├── lv_init()
 │
 ├── sdl_hal_init()
 │
 ├── ui_init()
 │
 └── application loop
```

O HAL (`hal/`) encapsula a integração da plataforma necessária para executar LVGL no ambiente atual.

Essa camada é especialmente relevante porque a implementação de plataforma do PC não será necessariamente a mesma do ESP32-S3.

O ambiente de desenvolvimento no PC é documentado em [09-pc-development-environment.md](../09-pc-development-environment.md).

---

### 3. Fronteira PC → ESP32

A arquitetura pode ser visualizada como:

```text
                 APLICAÇÃO
┌──────────────────────────────────────┐
│                                      │
│                UI                    │
│                                      │
│              DOMAIN                  │
│                                      │
│        Hardware Interfaces           │
│                                      │
└──────────────────┬───────────────────┘
                   │
          ┌────────┴─────────┐
          │                  │
          ▼                  ▼
      SIMULADOR           HARDWARE
          │                  │
       Windows           ESP32-S3
          │                  │
        SDL2          Display / HX711
                         / Servo
```

A fronteira entre o que é comum e o que é específico deve ser preservada durante a migração. Ver [11-migration-pc-to-esp32.md](../11-migration-pc-to-esp32.md).

---

### 4. Diretriz para novos códigos

Antes de adicionar um novo código, deve-se perguntar:

**É interface visual?**

Provavelmente pertence a:

```text
src/ui/
```

**É uma regra do funcionamento da dosagem?**

Provavelmente pertence a:

```text
src/domain/
```

**É uma abstração de dispositivo?**

Provavelmente pertence a:

```text
src/hardware/*.h
```

**É implementação de um dispositivo específico?**

Provavelmente pertence a:

```text
src/hardware/
```

ou a uma camada específica de plataforma que venha a ser criada.

**É integração com a plataforma?**

Provavelmente pertence a:

```text
src_pc/hal/
```

---

### 5. Regra contra acoplamento

Ao adicionar uma dependência, verificar:

```text
Esse código precisa realmente conhecer a camada abaixo?
```

E também:

```text
Existe uma abstração apropriada que deveria ser usada?
```

Exemplo:

```text
dosing_controller.c
```

não deve começar a incluir:

```c
#include <SDL.h>
```

ou:

```c
#include "driver/hx711.h"
```

apenas porque precisa obter o peso ou controlar o dispenser.

Esses detalhes devem permanecer atrás das abstrações correspondentes.