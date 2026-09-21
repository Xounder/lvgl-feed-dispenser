## 06. Explicacao - simulacao, sensores, maquina de estados e docs

[voltar ao índice](../implementation.md)

---

# 7. A simulação entrou antes do que o plano previa

O plano dizia:

```text
1 UI
2 ScreenManager
3 Home
4 Mode
5 Config
6 Dosing
7 Completed
8 Eventos
9 App
10 Estados
11 Sensor
12 Dispenser
13 Dosagem simulada
14 Hardware
```

Na prática fizemos algo mais próximo de:

```text
UI
 ↓
ScreenManager
 ↓
telas
 ↓
navegação
 ↓
Domain
 ↓
WeightSensor
 ↓
Dispenser
 ↓
simulação
 ↓
integração UI + domínio
```

E isso foi uma boa decisão porque permitiu verificar rapidamente:

```text
configuração
   ↓
início
   ↓
peso aumentando
   ↓
meta
   ↓
parar
   ↓
concluído
```

em vez de termos várias telas prontas sem comportamento.

---

# 8. O sensor simulado e dispenser simulado seguiram a proposta

A ideia original:

```text
WeightSensor
      │
      ▼
SimulatedWeightSensor
```

Hoje:

```c
WeightSensor simulated_weight_sensor
```

E:

```text
Dispenser
      │
      ▼
SimulatedDispenser
```

Hoje:

```c
Dispenser simulated_dispenser
```

Então essa parte foi seguida diretamente.

---

# 9. A dosagem simulada ficou até mais concreta que o plano

O plano dizia algo como:

```text
0
↓
12
↓
27
↓
43
↓
68
↓
...
↓
150
```

Nós implementamos:

```text
Dosing timer
     ↓
controller_update()
     ↓
dispenser ativo?
     ↓
+2 g
     ↓
nova leitura
```

Então temos um ciclo real de simulação.

E quando:

```c
current_weight >= target_grams
```

ocorre:

```text
STOP DISPENSER
      ↓
COMPLETED
      ↓
COMPLETED SCREEN
```

Isso é exatamente o comportamento que o plano pretendia validar.

---

# 10. Também fizemos uma coisa que o plano original ainda não tinha formalizado tão bem

Criamos uma separação entre:

```text
DosingConfig
```

e:

```text
DosingController
```

Hoje temos:

```text
DosingConfig
├── mode (DOSING_MODE_GRAMS / DOSING_MODE_CURRENCY)
├── target_grams
├── target_money_cents
└── price_per_kg_cents
```

e:

```text
DosingController
├── start
├── update
├── cancel (interrupção → INTERRUPTED)
├── new_dosing (tara)
├── manual_release_start / manual_release_stop
├── get_weight
├── get_state
└── get_phase
```

Isso deixou a evolução posterior bem mais clara.

---

# 11. A máquina de estados ficou parcialmente implementada

O plano falava em:

```text
IDLE
SELECT_MODE
CONFIGURING
DOSING
COMPLETED
```

e posteriormente:

```text
ERROR
```

Hoje o domínio possui:

```c
typedef enum {
    DOSING_STATE_IDLE,
    DOSING_STATE_DOSING,
    DOSING_STATE_COMPLETED,
    DOSING_STATE_INTERRUPTED
} DosingState;
```

Então:

```text
IDLE         ✅
DOSING       ✅
COMPLETED    ✅
INTERRUPTED  ✅

SELECT_MODE    → representado atualmente pela UI
CONFIGURING   → representado atualmente pela UI
ERROR         → planejado
```

Isso é uma distinção importante.

Não abandonamos a ideia.

**Ainda não terminamos essa parte.**

---

# 12. E isso explica por que os documentos que criamos depois ficaram diferentes do plano inicial

Quando fomos documentando o projeto, começamos a separar claramente:

```text
CURRENT
```

de:

```text
PLANNED
```

Por exemplo:

```text
Atual:

IDLE → DOSING → COMPLETED
             └→ INTERRUPTED
```

e:

```text
Objetivo:

IDLE
 ↓
SELECT_MODE
 ↓
CONFIGURING
 ↓
DOSING
 ├── COMPLETED
 ├── INTERRUPTED
 └── ERROR
```

Isso foi importante para não fingir que uma arquitetura planejada já estava implementada.

---
