## 1. Objetivo

_Voltar ao índice: [`../05-hardware-abstraction.md`](../05-hardware-abstraction.md)._

---

Este documento descreve como o projeto representa sensores e atuadores através de abstrações de hardware.

O objetivo principal é permitir que a lógica de dosagem seja desenvolvida e testada sem depender diretamente do hardware físico.

Atualmente, o projeto possui duas abstrações principais:

```text
WeightSensor
Dispenser
```

Cada uma representa uma capacidade que o domínio precisa utilizar.

As implementações atuais são simuladas:

```text
SimulatedWeightSensor
SimulatedDispenser
```

No futuro, deverão existir implementações reais para:

```text
HX711 + Load Cell
SG90 / atuador
```

A ideia central é:

```text
DosingController
       │
       ├───────────────┐
       ▼               ▼
 WeightSensor       Dispenser
       │               │
   ┌───┴───┐       ┌───┴───┐
   ▼       ▼       ▼       ▼
Simulado  Real  Simulado  Real
```

---

## 2. Princípio da abstração

O domínio não deve depender de um componente físico específico.

O `DosingController` precisa saber apenas que existe:

```text
um sensor capaz de informar o peso
```

e:

```text
um mecanismo capaz de liberar/interromper a ração
```

Ele não precisa saber se o sensor é:

```text
simulado
HX711
outro sensor
```

nem se o dispenser é:

```text
simulado
SG90
outro atuador
```

---

## 3. Estrutura conceitual

A arquitetura é:

```text
                    DOMAIN
                       │
              ┌────────┴────────┐
              │                 │
              ▼                 ▼
        WeightSensor         Dispenser
              │                 │
       ┌──────┴──────┐    ┌─────┴─────┐
       ▼             ▼    ▼           ▼
   Simulated       Real Simulated    Real
       │             │    │           │
       ▼             ▼    ▼           ▼
    PC test        HX711  PC test    SG90
```

Essa separação permite que o mesmo comportamento de domínio seja executado em diferentes ambientes.

---

## 4. Localização no projeto

As abstrações ficam em:

```text
src/hardware/
```

Estrutura atual:

```text
src/hardware/
├── weight_sensor.h
├── simulated_weight_sensor.c
├── dispenser.h
└── simulated_dispenser.c
```

A intenção é manter os contratos separados das implementações.