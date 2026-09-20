## 09. Estrutura após a migração e duas plataformas

[voltar ao índice](../11-migration-pc-to-esp32.md)

---

### 58. Migração da simulação

Mesmo depois da implementação real, o simulador deve continuar disponível.

Idealmente:

```text
Build PC
 ↓
simulated hardware
```

e:

```text
Build ESP32
 ↓
real hardware
```

compartilham:

```text
domain
UI
interfaces
configurações conceituais
```

Isso permite testar mudanças sem precisar sempre utilizar o dispositivo físico.

O papel do simulador como plataforma permanente é canônico em [06-simulation-strategy.md](../06-simulation-strategy.md).

---

### 59. Possível estrutura após a migração

A estrutura poderá evoluir para algo semelhante a:

```text
src/
├── domain/
│   ├── dosing_config.h
│   ├── dosing_controller.c
│   └── dosing_controller.h
│
├── ui/
│   ├── ui.c
│   ├── screen_manager.c
│   └── screens/
│
└── hardware/
    ├── weight_sensor.h
    ├── dispenser.h
    ├── simulated/
    │   ├── simulated_weight_sensor.c
    │   └── simulated_dispenser.c
    │
    └── esp32/
        ├── real_weight_sensor.c
        └── real_dispenser.c
```

A estrutura exata pode mudar.

O importante é manter separadas as implementações.

---

### 60. PC e ESP32 como duas plataformas

Uma visão mais completa:

```text
                 Shared Application
                        │
          ┌─────────────┴─────────────┐
          │                           │
          ▼                           ▼
      PC Platform                ESP32 Platform
          │                           │
      SDL2/HAL                    ESP-IDF/HAL
          │                           │
   Simulated HW                   Real HW
          │                           │
     ┌────┴────┐                ┌─────┴─────┐
     │         │                │           │
   Weight   Dispenser         HX711       Servo
```

---

### 61. O que deve permanecer independente

Idealmente, o seguinte código não deve depender diretamente de ESP-IDF:

```text
domain/
```

E quanto mais possível:

```text
ui/
```

deve depender somente das APIs do LVGL e das interfaces da aplicação, e não diretamente de GPIO, PWM ou HX711.

---

### 62. O que pode depender do ESP-IDF

A camada específica do ESP32 pode conhecer:

```text
GPIO
PWM
I2C
SPI
ADC
timers
tasks
queues
NVS
drivers
ESP-IDF APIs
```

Esses detalhes pertencem à plataforma.

---

### 63. Regra de dependência

A direção desejada continua sendo:

```text
UI
 ↓
Domain
 ↓
Hardware abstractions
 ↓
Platform implementation
```

Evitar:

```text
Domain
 ↓
ESP-IDF
 ↓
GPIO
 ↓
HX711
```

diretamente.

---

### 64. O hardware deve implementar o domínio

O princípio continua:

> **O hardware deve implementar as necessidades da aplicação; a aplicação não deve ser moldada desnecessariamente pelos detalhes do hardware.**

Por exemplo, a aplicação precisa:

```text
obter peso
```

Então existe:

```text
WeightSensor
```

A aplicação precisa:

```text
acionar dispenser
```

Então existe:

```text
Dispenser
```

O fato de o hardware utilizar HX711 ou SG90 não deve vazar para o domínio.

---

### 65. Quando adaptar a arquitetura

A migração pode revelar que uma abstração criada no PC é insuficiente.

Isso é esperado.

Por exemplo, o hardware real pode exigir:

```text
tare()
is_stable()
get_status()
```

Nesse caso, a interface pode evoluir.

Porém, a mudança deve ser motivada por uma necessidade real do hardware ou do domínio, e não simplesmente por antecipação.

---

### 66. Não fazer uma abstração perfeita antecipadamente

O projeto ainda está em desenvolvimento.

Portanto, não é necessário definir hoje uma interface gigantesca contendo todas as possibilidades futuras.

É preferível:

```text
necessidade real
 ↓
interface mínima
 ↓
implementação
 ↓
teste
 ↓
evolução
```

Isso mantém o projeto mais simples.

---

### 67. Migração do estado atual

Atualmente:

```text
PC
 ↓
LVGL
 ↓
SDL2
 ↓
simulated_weight_sensor
 ↓
simulated_dispenser
```

Depois:

```text
ESP32-S3
 ↓
LVGL
 ↓
display/touch
 ↓
real_weight_sensor
 ↓
real_dispenser
```

O `DosingController` deve continuar sendo a peça que coordena o processo.