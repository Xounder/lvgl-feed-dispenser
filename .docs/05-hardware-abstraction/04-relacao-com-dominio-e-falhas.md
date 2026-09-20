## 28. Relação entre domínio e hardware

_Voltar ao índice: [`../05-hardware-abstraction.md`](../05-hardware-abstraction.md)._

---

O fluxo desejado é:

```text
              DosingController
                     │
             ┌───────┴───────┐
             ▼               ▼
       WeightSensor       Dispenser
             │               │
             ▼               ▼
          peso atual       liberar/parar
```

O controller combina as informações:

```text
peso atual
+
objetivo
+
estado do dispenser
```

para determinar o próximo passo.

---

## 29. Exemplo de ciclo

Supondo:

```text
objetivo = 100 g
```

O ciclo conceitual é:

```text
reset/tara
    ↓
start dispenser
    ↓
read weight
    ↓
20 g
    ↓
read weight
    ↓
45 g
    ↓
read weight
    ↓
78 g
    ↓
read weight
    ↓
100 g
    ↓
stop dispenser
    ↓
COMPLETED
```

---

## 30. Exemplo com overshoot

No hardware real, pode acontecer:

```text
objetivo = 100 g

leitura = 96 g
      ↓
dispenser continua
      ↓
leitura seguinte = 103 g
      ↓
stop
```

A regra atual de domínio:

```text
weight >= target
```

permite essa situação.

O controle fino do overshoot poderá ser melhorado posteriormente com base no comportamento físico observado.

A regra `weight >= target` pertence ao domínio (ver [`../04-domain-and-state-machine.md`](../04-domain-and-state-machine.md)).

---

## 31. Erros do sensor

A abstração deverá permitir futuramente distinguir:

```text
peso válido
```

de:

```text
peso inválido
```

ou:

```text
sensor indisponível
```

A interface atual ainda é simples demais para representar explicitamente todos esses casos.

Uma evolução possível seria retornar um status junto com o valor.

Por exemplo, conceitualmente:

```text
WeightReading
├── value
├── valid
└── error
```

Isso é uma possibilidade futura, não uma obrigação imediata.

---

## 32. Erros do dispenser

Da mesma forma, a implementação real poderá precisar informar:

```text
atuador funcionando
```

ou:

```text
falha
```

A interface atual possui apenas:

```text
is_active()
```

Isso não é necessariamente suficiente para diagnosticar falhas físicas.

A interface deve evoluir somente quando houver necessidade real de representar essas condições.

---

## 33. Simulação de falhas

Uma das vantagens da abstração é permitir criar falhas artificialmente.

Por exemplo:

### Sensor travado

```text
read_grams()
    ↓
sempre retorna 20 g
```

### Sensor inválido

```text
read_grams()
    ↓
erro
```

### Dispenser travado

```text
stop()
    ↓
simulação permanece ativa
```

### Fluxo lento

```text
+0.5 g por atualização
```

Isso permitirá testar o controller sem depender de provocar essas falhas fisicamente.

---

## 34. Simulação versus implementação real

| Capacidade      | Simulada                    | Real                       |
| --------------- | --------------------------- | -------------------------- |
| Leitura de peso | variável interna            | HX711 + Load Cell          |
| Reset           | zera variável               | tara/referência            |
| Aumento do peso | artificial                  | efeito físico da ração     |
| Dispenser       | estado booleano             | SG90/atuador               |
| Start           | ativa flag                  | movimenta mecanismo        |
| Stop            | desativa flag               | fecha/interrompe mecanismo |
| Ruído           | opcional futuro             | naturalmente presente      |
| Calibração      | não necessária inicialmente | necessária                 |
| Falhas físicas  | simuladas                   | reais                      |

---

## 35. O que não deve vazar para o domínio

Os seguintes detalhes devem permanecer encapsulados:

```text
HX711
GPIO
PWM
ADC/raw values
fator de calibração
tare implementation
servo angle
servo duty cycle
pino físico
timer de hardware
driver específico
```

O domínio deve trabalhar com:

```text
peso
estado
start
stop
ativo
erro
```

ou outras capacidades de nível equivalente.