# Simulação atual e comportamento físico

## 6. Simulação atual do peso

Atualmente o peso é representado por uma variável:

```c
static int simulated_weight = 0;
```

A leitura retorna:

```c
simulated_read_grams()
```

e o controller aumenta artificialmente o peso:

```c
simulated_weight_sensor.add_grams(2);
```

A cada atualização:

```text
+2 g
```

Quando o objetivo é:

```text
100 g
```

o comportamento é aproximadamente:

```text
0
2
4
6
8
...
98
100
```

---

## 7. Por que começar com uma simulação simples

A simulação inicial deliberadamente não tenta reproduzir o mundo físico.

Isso permite validar primeiro a regra fundamental:

```text
se peso >= objetivo:
    parar dispenser
    concluir dosagem
```

Se essa regra básica não estiver correta, adicionar ruído, calibração ou física não resolverá o problema.

Portanto:

```text
lógica correta
      ↓
simulação mais realista
      ↓
hardware real
```

é preferível a:

```text
hardware complexo
      ↓
tentar descobrir a lógica depois
```

---

## 8. Relação entre dispenser e peso

Na simulação, o dispenser representa a causa do aumento de peso.

Conceitualmente:

```text
Dispenser ativo
      ↓
ração sendo liberada
      ↓
peso aumenta
```

Isso permite reproduzir a relação física sem precisar de uma célula de carga.

No hardware real:

```text
SG90 / mecanismo
      ↓
ração
      ↓
Load Cell
      ↓
HX711
      ↓
peso
```

No simulador:

```text
SimulatedDispenser
      ↓
SimulatedWeightSensor
      ↓
peso simulado
```

Os contratos de `WeightSensor` e `Dispenser` (e suas versões simuladas) estão definidos em [05-hardware-abstraction.md](../05-hardware-abstraction.md).

---

## 9. O simulador não deve criar uma relação artificial no domínio

Embora a simulação precise relacionar dispenser e peso, essa relação não deve contaminar o domínio.

O controller deve continuar pensando:

```text
ler peso
```

e:

```text
controlar dispenser
```

A regra:

```text
dispenser ativo → peso aumenta
```

é uma característica do ambiente simulado.

No mundo físico, isso acontece naturalmente.

O domínio, com suas regras e estados, é descrito em [04-domain-and-state-machine.md](../04-domain-and-state-machine.md).

---

## 10. Taxa de alimentação

A simulação atual adiciona:

```text
2 g
```

a cada atualização de aproximadamente:

```text
300 ms
```

Isso corresponde aproximadamente a:

```text
6,7 g/s
```

Essa taxa pode posteriormente ser transformada em uma variável configurável.

Por exemplo:

```text
fluxo = 5 g/s
```

ou:

```text
fluxo = 10 g/s
```

permitindo testar diferentes mecanismos.

---

## 11. Por que a taxa deve ser configurável

Um mecanismo físico pode liberar quantidades diferentes de ração dependendo de:

* abertura;
* tipo de ração;
* geometria;
* posição do servo;
* quantidade no reservatório;
* obstruções;
* vibração.

Portanto, assumir uma única taxa fixa no simulador limita os testes.

Uma simulação futura poderia permitir:

```text
cenário lento
    2 g/s

cenário normal
    7 g/s

cenário rápido
    15 g/s
```

---

## 12. Simulação baseada em tempo

Uma evolução importante é fazer o peso depender do tempo.

Em vez de:

```text
cada update = +2 g
```

usar:

```text
delta_weight = flow_rate × delta_time
```

Por exemplo:

```text
flow_rate = 7 g/s
delta_time = 0,3 s

delta_weight = 7 × 0,3
             = 2,1 g
```

Isso torna a simulação menos dependente da frequência do timer da UI.

---

## 13. Separação entre tempo e atualização da UI

Atualmente o timer da tela de dosagem chama:

```text
dosing_controller_update()
```

A simulação funciona porque o intervalo é aproximadamente constante.

No futuro, é preferível que a simulação considere o tempo real transcorrido.

Conceitualmente:

```text
controller_update(delta_time)
```

ou:

```text
simulation_update(current_time)
```

Isso permite que o comportamento seja consistente mesmo se:

```text
o frame rate variar
```

ou:

```text
o computador ficar ocupado
```

---

## 14. Ruído do sensor

Uma célula de carga real não deve ser considerada perfeitamente estável.

Uma leitura poderia ser:

```text
100,0 g
100,3 g
99,8 g
100,2 g
99,9 g
```

Mesmo com a quantidade física praticamente constante.

A simulação poderá adicionar uma pequena variação:

```text
peso_real
    +
ruído
    ↓
peso_lido
```

Por exemplo:

```text
peso real = 100 g

leituras:
99,8
100,2
100,1
99,9
100,3
```

---

## 15. Ruído não deve ser adicionado cedo demais

Ruído pode dificultar a depuração.

Por isso, inicialmente:

```text
ruído = 0
```

é preferível.

Depois que o comportamento determinístico estiver funcionando, pode-se ativar:

```text
ruído pequeno
```

para verificar se as regras continuam funcionando.