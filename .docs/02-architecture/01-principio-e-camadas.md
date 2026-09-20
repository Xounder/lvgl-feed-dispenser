## 01. Princípio e camadas

[voltar ao índice](../02-architecture.md)

---

### 1. Objetivo

Este documento descreve a arquitetura do projeto, suas camadas, responsabilidades, dependências e fluxo de comunicação.

A arquitetura foi construída para permitir que o mesmo núcleo de lógica seja utilizado tanto no **simulador desktop** quanto, posteriormente, no **ESP32-S3**.

A principal preocupação arquitetural é evitar que:

* a UI contenha regras de negócio;
* o domínio conheça detalhes do hardware;
* o hardware conheça detalhes da UI;
* a implementação do simulador seja confundida com a implementação definitiva do dispositivo.

A estrutura conceitual é:

```text
┌──────────────────────────────────────┐
│                 UI                   │
│              LVGL / SDL2             │
└──────────────────┬───────────────────┘
                   │
                   │ comandos / consulta
                   ▼
┌──────────────────────────────────────┐
│               DOMAIN                 │
│          DosingController             │
│          DosingConfig                 │
│          DosingState                  │
└──────────────────┬───────────────────┘
                   │
                   │ interfaces
                   ▼
┌──────────────────────────────────────┐
│       HARDWARE ABSTRACTION           │
│          WeightSensor                 │
│          Dispenser                   │
└──────────────────┬───────────────────┘
                   │
             ┌─────┴─────┐
             ▼           ▼
      ┌────────────┐ ┌────────────┐
      │ Simulated  │ │    Real    │
      │ Hardware   │ │  Hardware  │
      └────────────┘ └────────────┘
```

---

### 2. Princípio arquitetural central

O princípio mais importante do projeto é:

> **A lógica de negócio deve depender de abstrações, e não de detalhes específicos de UI, plataforma ou hardware.**

Em termos práticos:

```text
ERRADO:

DosingController
      ↓
LVGL
      ↓
GPIO
      ↓
Servo
```

O controller não deve conhecer esses detalhes.

A direção desejada é:

```text
UI
 ↓
DosingController
 ↓
Hardware Interfaces
 ↓
Hardware Implementation
```

Isso permite trocar uma implementação sem alterar as regras principais do sistema.

---

### 3. Camadas

A arquitetura atual pode ser dividida em três camadas principais:

```text
┌──────────────────────────────┐
│            UI                │
├──────────────────────────────┤
│          DOMAIN              │
├──────────────────────────────┤
│    HARDWARE ABSTRACTION      │
├──────────────────────────────┤
│   HARDWARE IMPLEMENTATION    │
└──────────────────────────────┘
```

No simulador, a última camada é composta pelas implementações simuladas.

No dispositivo final, será composta pelas implementações reais.

---

### 4. Dependências entre camadas

A direção desejada das dependências é:

```text
UI
 │
 ▼
DOMAIN
 │
 ▼
HARDWARE INTERFACES
 │
 ▼
IMPLEMENTATIONS
```

Representando graficamente:

```text
┌───────────────┐
│      UI       │
│ LVGL / SDL2   │
└───────┬───────┘
        │
        ▼
┌───────────────┐
│    DOMAIN     │
│               │
│ Controller    │
│ Config        │
│ State         │
└───────┬───────┘
        │
        ▼
┌────────────────────┐
│ Hardware Interfaces│
│                    │
│ WeightSensor       │
│ Dispenser          │
└─────────┬──────────┘
          │
       ┌──┴───┐
       ▼      ▼
   Simulated Real
```

---

### 5. Dependências que devem ser evitadas

As seguintes dependências são consideradas indesejáveis:

```text
DOMAIN → LVGL
DOMAIN → SDL2
DOMAIN → ESP32 SDK
DOMAIN → GPIO
DOMAIN → HX711 driver
DOMAIN → Servo driver
```

Também deve ser evitado:

```text
Hardware → UI
```

O hardware não deve precisar saber que existe uma tela específica.

---

### 6. Regra de dependência

Uma regra prática para novos componentes:

> **Quanto mais próximo da regra de negócio um componente estiver, menos detalhes de plataforma ele deve conhecer.**

Assim:

```text
Mais independente
        ↑
        │
   DosingConfig
   DosingController
   Domain logic
        │
   Hardware interfaces
        │
   Hardware drivers
        │
   GPIO / PWM / SDK
        ↓
Mais específico
```

A UI também é específica da plataforma de apresentação, mas permanece separada do domínio.