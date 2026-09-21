# Arquitetura da simulação

## 34. Independência da UI

A simulação não deve depender de elementos visuais.

Evitar colocar regras como:

```text
se progress_bar == 100%
```

para controlar o hardware simulado.

O fluxo correto continua sendo:

```text
simulação
   ↓
hardware abstraction
   ↓
controller
   ↓
estado
   ↓
UI
```

e não:

```text
UI
   ↓
simulação
```

---

## 35. Independência do SDL2

O SDL2 existe para fornecer o ambiente gráfico do simulador.

Ele não deve fazer parte da lógica da simulação.

A arquitetura deve permanecer aproximadamente:

```text
SDL2
 ↓
LVGL
 ↓
UI
 ↓
Domain
 ↓
Hardware abstraction
 ↓
Simulation
```

A simulação deve poder evoluir sem depender de APIs específicas do SDL2.

---

## 36. Simulação e relógio

O tempo é uma dependência importante.

No simulador atual, o tempo é indiretamente controlado pelo loop:

```c
lv_timer_handler();
```

e pelos timers do LVGL.

No futuro, a lógica de simulação poderá utilizar uma fonte de tempo explícita.

Conceitualmente:

```text
SimulationClock
```

poderia fornecer:

```text
elapsed_time
current_time
delta_time
```

Isso permitiria desacoplar o comportamento físico da taxa de atualização da interface.

---

## 37. Não simular o que não influencia o software

O simulador não precisa reproduzir:

```text
todos os detalhes elétricos
```

nem:

```text
todos os detalhes mecânicos
```

se eles não influenciam o comportamento da aplicação.

Por exemplo, não é necessário simular:

```text
corrente elétrica do servo
```

se o objetivo do teste é verificar:

```text
o controller para quando o peso atinge o objetivo?
```

A simulação deve ser orientada ao comportamento relevante.

---

## 38. Fidelidade versus complexidade

Existe uma relação:

```text
mais fidelidade
      ↓
mais complexidade
      ↓
mais dificuldade de depuração
```

Portanto, a pergunta não deve ser:

> "Como reproduzir perfeitamente o hardware?"

A pergunta deve ser:

> "Qual comportamento do hardware precisamos representar para validar o software?"

Essa distinção deve orientar a evolução do simulador.

---

## 39. Simulação como contrato comportamental

O simulador também pode servir para definir expectativas.

Por exemplo:

```text
Dado:
target = 100 g
fluxo = 7 g/s
sem falhas

Quando:
dispenser inicia

Então:
peso aumenta

E quando:
peso >= 100 g

Então:
dispenser para
estado = COMPLETED
```

Esse formato aproxima a simulação de testes comportamentais.

---

## 40. Testes de regressão

Depois que determinados comportamentos forem validados, eles podem se tornar cenários de regressão.

Por exemplo:

```text
TEST: dosagem normal
TEST: cancelamento
TEST: objetivo mínimo
TEST: overshoot
TEST: ausência de fluxo
TEST: timeout
```

Alterações futuras no controller devem continuar passando esses cenários.

Isso reduz o risco de uma melhoria quebrar um comportamento já validado.

---

## 41. Relação com o hardware real

O objetivo final não é garantir que:

```text
simulação = hardware
```

em todos os detalhes.

O objetivo é garantir que:

```text
regras de negócio
```

e:

```text
contratos de hardware
```

sejam suficientemente bem definidos para que a implementação real possa substituí-los.

Quando o hardware chegar:

```text
SimulatedWeightSensor
```

será substituído por algo equivalente a:

```text
HX711WeightSensor
```

e:

```text
SimulatedDispenser
```

por algo equivalente a:

```text
ServoDispenser
```

Essas substituições seguem os contratos definidos em [05-hardware-abstraction.md](../05-hardware-abstraction.md).

---

## 42. Diferenças esperadas entre simulação e hardware

Mesmo com uma boa simulação, diferenças serão inevitáveis.

Por exemplo:

```text
SIMULAÇÃO
peso aumenta de forma controlada

HARDWARE
peso oscila
```

ou:

```text
SIMULAÇÃO
servo para imediatamente

HARDWARE
mecanismo possui inércia
```

ou:

```text
SIMULAÇÃO
fluxo = 7 g/s

HARDWARE
fluxo varia com a ração
```

Essas diferenças não significam que o simulador falhou.

Elas representam a natureza aproximada da simulação.

---

## 43. O que deve permanecer igual

Apesar das diferenças físicas, determinados comportamentos devem permanecer consistentes.

Por exemplo:

```text
objetivo atingido
      ↓
parar dispenser
      ↓
COMPLETED
```

e:

```text
interrupção
      ↓
parar dispenser
      ↓
INTERRUPTED
```

e:

```text
liberação manual solta
      ↓
parar dispenser
      ↓
LED apaga
```

e futuramente:

```text
falha crítica
      ↓
parar dispenser
      ↓
ERROR
```

Essas regras pertencem ao comportamento da aplicação.

---

## 44. Arquitetura futura da simulação

Uma evolução possível é:

```text
src/
└── simulation/
    ├── simulation.c
    ├── simulation.h
    ├── simulation_config.h
    ├── simulation_clock.c
    └── scenarios/
        ├── normal.c
        ├── slow_flow.c
        ├── sensor_failure.c
        └── dispenser_failure.c
```

Isso é uma direção futura, não uma estrutura obrigatória imediata.

A estrutura atual permanece simples:

```text
src/hardware/
├── weight_sensor.h
├── dispenser.h
├── simulated/
│   ├── simulated_weight_sensor.c
│   └── simulated_dispenser.c
└── esp32/
    ├── real_weight_sensor.cpp
    └── real_dispenser.cpp
```