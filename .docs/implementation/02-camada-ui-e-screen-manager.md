## 02. Camada ui, ScreenManager e separacao da logica

[voltar ao índice](../implementation.md)

---

# 2. Criamos primeiro a camada `ui`

Antes de colocar HX711, servo, simulação etc., vamos fazer as telas.

Algo assim:

```text
UI
│
├── Home
│
├── Seleção de modo
│
├── Configuração
│
├── Dosagem
│
└── Concluído
```

E o fluxo:

```text
             ┌─────────────┐
             │    HOME     │
             └──────┬──────┘
                    │
                    ▼
          ┌──────────────────┐
          │ SELECIONAR MODO  │
          └────────┬─────────┘
                   │
                   ▼
          ┌──────────────────┐
          │   CONFIGURANDO   │
          └────────┬─────────┘
                   │
                   ▼
          ┌──────────────────┐
          │     DOSANDO      │
          └───────┬──────────┘
                  │
                  ▼
          ┌──────────────────┐
          │    CONCLUÍDO     │
          └──────────────────┘
```

---

# 3. Teremos um `ScreenManager`

Eu acho particularmente útil para esse projeto.

Por exemplo:

```c
typedef enum {
    SCREEN_HOME,
    SCREEN_MODE,
    SCREEN_CONFIG,
    SCREEN_DOSING,
    SCREEN_COMPLETED
} Screen;
```

E:

```c
void ui_show_screen(Screen screen);
```

Assim uma tela não precisa saber como destruir/criar outra.

Por exemplo:

```c
ui_show_screen(SCREEN_MODE);
```

e o `ui` decide o que fazer.

---

# 4. Não misturar tela com lógica da dosagem

Isso é importante para sua futura migração para ESP32.

Por exemplo, **não queremos** isso:

```c
static void start_button_event(lv_event_t *e)
{
    servo_set_angle(90);
    hx711_read();
    ...
}
```

A UI deveria fazer algo mais parecido com:

```c
static void start_button_event(lv_event_t *e)
{
    app_start_dosing();
}
```

E a aplicação decide o que acontece.

Então:

```text
        UI
         │
         │ evento
         ▼
       APP
         │
         ▼
      DOMAIN
       /   \
      /     \
 weight    dispenser
 sensor      │
```

Isso vai facilitar muito quando trocarmos:

```text
SimulatedWeightSensor
```

por:

```text
HX711WeightSensor
```

e:

```text
SimulatedDispenser
```

por:

```text
ServoDispenser
```

---
