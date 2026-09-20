## 28. Timeout

_Voltar ao índice: [`../04-domain-and-state-machine.md`](../04-domain-and-state-machine.md)._

---

Um problema possível é:

```text
DOSING
  ↓
peso nunca aumenta
```

Sem timeout, o sistema poderia permanecer indefinidamente no estado de dosagem.

Por isso, a arquitetura deve permitir futuramente:

```text
tempo máximo
```

ou:

```text
tempo sem progresso
```

Exemplo:

```text
DOSING
  ↓
peso não aumenta por X segundos
  ↓
ERROR
```

O valor de `X` deve ser definido a partir de testes reais e não arbitrariamente.

---

## 29. Falta de progresso

Além do timeout absoluto, pode ser útil verificar progresso.

Exemplo:

```text
peso anterior = 50 g
peso atual    = 50 g
```

repetidamente.

Isso pode indicar:

* dispenser vazio;
* mecanismo travado;
* sensor com problema;
* ração bloqueada.

Uma futura regra poderá ser:

```text
se dispenser está ativo
e peso não aumenta durante determinado intervalo
→ ERROR
```

Essa regra ainda não faz parte da implementação atual.

---

## 30. Overshoot

No mundo físico, pode ocorrer:

```text
objetivo = 100 g
```

mas a leitura seguinte ser:

```text
102 g
```

Isso não significa necessariamente que o controller falhou.

A regra atual é:

```text
peso >= objetivo
```

e não:

```text
peso == objetivo
```

Portanto:

```text
100 g → concluído
101 g → concluído
102 g → concluído
```

A quantidade real de overshoot deverá ser avaliada durante a integração física.

---

## 31. Controle do dispenser

O controller deve tratar o dispenser como uma capacidade:

```text
start
stop
is_active
```

A decisão de quando iniciar e parar pertence ao domínio.

A forma como o dispenser realiza fisicamente essa operação pertence à implementação (ver [`../05-hardware-abstraction.md`](../05-hardware-abstraction.md)).

Assim:

```text
Domain:
    "pare o dispenser"

Hardware:
    "como parar o servo"
```

---

## 32. Leitura do peso

O controller deve consumir uma representação útil do peso:

```text
gramas
```

No simulador:

```text
simulated_weight_sensor.read_grams()
```

No hardware, a cadeia (`HX711 → raw reading → calibration/filtering → grams → WeightSensor → Controller`) pertence à abstração de hardware (ver [`../05-hardware-abstraction.md`](../05-hardware-abstraction.md)).

A conversão de leitura bruta para gramas deve permanecer na implementação do sensor.

---

## 33. Atualização do processo

A atualização do processo pode ser entendida como:

```text
dosing_controller_update()
```

Durante `DOSING`, essa função deve:

1. verificar o estado atual;
2. obter a leitura de peso;
3. avaliar a condição de conclusão;
4. controlar o dispenser;
5. detectar possíveis erros;
6. atualizar o estado.

No estado atual, a implementação ainda é simplificada.

A direção futura é concentrar cada vez mais as regras de controle nessa camada.