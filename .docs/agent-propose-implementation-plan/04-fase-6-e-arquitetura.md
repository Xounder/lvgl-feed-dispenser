## 04. Fase 6 - maquina de estados, arquitetura recomendada e fluxo

[voltar ao índice](../agent-propose-implementation-plan.md)

---

# Fase 6 — Máquina de estados

Essa, para mim, deveria ser o **coração do projeto**.

Algo assim:

```
```

```
            ┌──────────────┐
            │   INÍCIO     │
            └──────┬───────┘
                   │
                   ▼
          ┌─────────────────┐
          │ AGUARDANDO MODO │
          └────────┬────────┘
                   │
         ┌─────────┴─────────┐
         ▼                   ▼
    ┌─────────┐         ┌─────────┐
    │ MASSA   │         │ VALOR   │
    └────┬────┘         └────┬────┘
         │                   │
         └─────────┬─────────┘
                   ▼
            ┌──────────────┐
            │ CONFIGURANDO │
            └──────┬───────┘
                   │
                   ▼
            ┌──────────────┐
            │   DOSANDO    │
            └──────┬───────┘
                   │
          ┌────────┴────────┐
          │                 │
          ▼                 ▼
    ┌───────────┐      ┌────────┐
    │ CONCLUÍDO │      │  ERRO  │
    └───────────┘      └────────┘
```

Em código:

```
```

```
enum class SystemState {

    IDLE,

    SELECT_MODE,

    CONFIGURING,

    DOSING,

    COMPLETED,

    ERROR
};
```

E:

```
```

```
SystemState currentState =
    SystemState::IDLE;
```

---

# A lógica poderia funcionar sem LVGL

Isso é importante.

Por exemplo:

```
```

```
void dosingLogic() {

    float currentWeight =
        weightSensor->getWeight();

    if (
        currentWeight >=
        targetWeight
    ) {

        dispenser->stop();

        currentState =
            SystemState::COMPLETED;
    }
}
```

Veja que essa lógica não sabe nada sobre:

```
```

```
ESP32
LCD
LVGL
HX711
Servo
```

Ela apenas sabe:

```
```

```
Preciso saber o peso
```

E:

```
```

```
Preciso controlar o dispensador
```

Isso torna o código muito mais fácil de testar.

---

# Arquitetura que eu recomendo para você

Eu organizaria o projeto assim:

```
```

```
ration-dispenser/

│
├── src/
│
│   ├── app/
│   │
│   │   ├── Application.cpp
│   │   └── Application.h
│
│   ├── domain/
│   │
│   │   ├── DosingController.cpp
│   │   ├── DosingController.h
│   │   │
│   │   ├── SystemState.h
│   │   └── DosingMode.h
│
│   ├── hardware/
│   │
│   │   ├── interfaces/
│   │   │
│   │   │   ├── IWeightSensor.h
│   │   │   └── IDispenser.h
│   │   │
│   │   ├── simulated/
│   │   │
│   │   │   ├── SimulatedWeightSensor.cpp
│   │   │   └── SimulatedDispenser.cpp
│   │   │
│   │   └── real/
│   │
│   │       ├── HX711WeightSensor.cpp
│   │       └── ServoDispenser.cpp
│
│   └── ui/
│
│       ├── ScreenManager.cpp
│       │
│       └── screens/
│
│           ├── HomeScreen.cpp
│           ├── ModeScreen.cpp
│           ├── DosingScreen.cpp
│           ├── CompletedScreen.cpp
│           └── ErrorScreen.cpp
│
└── tests/
```

---

# O fluxo seria este

## Hoje

Você executa:

```
```

```
PROGRAMA NO PC
```

Usando:

```
```

```
LVGL
+
SimulatedWeightSensor
+
SimulatedDispenser
```

Arquitetura:

```
```

```
        PC

        │

        ▼

      LVGL

        │

        ▼

 DosingController

      │      │

      ▼      ▼

Peso Fake   Servo Fake
```

---

## Depois

Quando comprar tudo:

```
```

```
        ESP32-S3

            │

            ▼

           LVGL

            │

            ▼

     DosingController

        │         │

        ▼         ▼

       HX711      Servo
        │
        ▼

 Célula de carga
```

A lógica central praticamente continua a mesma.

Você troca:

```
```

```
SimulatedWeightSensor
```

por:

```
```

```
HX711WeightSensor
```

E:

```
```

```
SimulatedDispenser
```

por:

```
```

```
ServoDispenser
```

---
