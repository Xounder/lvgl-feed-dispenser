## 01. Organizacao proposta e main.c pequeno

[voltar ao índice](../implementation.md)

---

Sim. Agora que a base está versionada, eu **pararia de mexer em Git por enquanto** e começaria a separar o simulador da aplicação de verdade.

Como seu objetivo é depois levar isso para o **ESP32-S3 + LVGL**, eu evitaria colocar toda a lógica diretamente no `main.c`.

### Organização que eu sugiro

Partindo do que você já tem:

```text
pc-vscode/
├── config/
│   └── FreeRTOSConfig.h
│
├── src/
│   ├── main.c
│   │
│   ├── hal/
│   │   ├── hal.c
│   │   └── hal.h
│   │
│   ├── app/
│   │   └── app.c
│   │
│   ├── domain/
│   │   ├── dosing_controller.c
│   │   ├── dosing_controller.h
│   │   ├── dosing_mode.h
│   │   └── system_state.h
│   │
│   ├── hardware/
│   │   ├── weight_sensor.h
│   │   ├── dispenser.h
│   │   │
│   │   └── simulated/
│   │       ├── simulated_weight_sensor.c
│   │       ├── simulated_weight_sensor.h
│   │       ├── simulated_dispenser.c
│   │       └── simulated_dispenser.h
│   │
│   └── ui/
│       ├── ui.c
│       ├── ui.h
│       │
│       ├── screens/
│       │   ├── home_screen.c
│       │   ├── home_screen.h
│       │   ├── mode_screen.c
│       │   ├── mode_screen.h
│       │   ├── dosing_screen.c
│       │   ├── dosing_screen.h
│       │   ├── completed_screen.c
│       │   └── completed_screen.h
│       │
│       └── components/
│           ├── header.c
│           ├── header.h
│           ├── button.c
│           └── button.h
│
├── lv_conf.h
├── CMakeLists.txt
└── ...
```

Mas **não criaria tudo agora**. Vamos evoluir por etapas.

---

## 1. `main.c` fica extremamente pequeno

Hoje ele está fazendo:

```text
LVGL init
↓
HAL init
↓
cria demo
↓
loop LVGL
```

Queremos chegar a:

```c
int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    lv_init();

    sdl_hal_init(800, 480);

    app_init();

    while(1) {
        uint32_t sleep_time_ms = lv_timer_handler();

        if(sleep_time_ms == LV_NO_TIMER_READY) {
            sleep_time_ms = LV_DEF_REFR_PERIOD;
        }

#ifdef _MSC_VER
        Sleep(sleep_time_ms);
#else
        usleep(sleep_time_ms * 1000);
#endif
    }

    return 0;
}
```

Ou seja, `main.c` não precisa saber **quais telas existem**.

---
