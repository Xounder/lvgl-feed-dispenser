# Overshoot, fluxo e progresso

## 16. Overshoot

Um dos comportamentos mais importantes a simular é o overshoot.

Exemplo:

```text
objetivo = 100 g
peso = 97 g
```

O dispenser continua ativo.

Na próxima leitura:

```text
peso = 103 g
```

Então:

```text
103 >= 100
```

e o sistema deve parar.

Esse comportamento pode ocorrer fisicamente porque a ração já está em movimento quando o sistema decide fechar o mecanismo.

---

## 17. Overshoot configurável

A simulação poderá permitir cenários como:

```text
sem overshoot
```

ou:

```text
overshoot pequeno
+1 a +3 g
```

ou:

```text
overshoot grande
+10 g
```

Isso permite avaliar como o controller reage.

---

## 18. A simulação não deve esconder o overshoot

É importante não fazer:

```text
if (weight > target)
    weight = target;
```

apenas para deixar a interface bonita.

Isso esconderia um comportamento importante do hardware real.

O valor observado deve poder ultrapassar o alvo.

Por exemplo:

```text
target = 100 g

98
101
```

é uma situação válida.

---

## 19. Fluxo lento

Outro cenário importante:

```text
objetivo = 100 g
```

mas o dispenser libera:

```text
0,5 g/s
```

A dosagem levará bastante tempo.

Isso permite verificar se:

* o controller continua funcionando;
* a UI continua atualizando;
* o progresso é exibido corretamente;
* o sistema detecta ausência de progresso quando necessário.

---

## 20. Ausência de fluxo

Um cenário ainda mais importante:

```text
dispenser ativo
```

mas:

```text
peso não aumenta
```

Isso pode representar:

* reservatório vazio;
* obstrução;
* servo com problema;
* mecanismo desconectado;
* ração presa.

O simulador deve permitir reproduzir isso.

---

## 21. Falta de progresso

Uma futura regra do controller pode detectar:

```text
dispenser ativo
+
peso não aumenta
+
tempo suficiente transcorrido
```

e produzir:

```text
ERROR
```

Por exemplo:

```text
DOSING
  ↓
dispenser ativo
  ↓
peso = 30 g
  ↓
peso = 30 g
  ↓
peso = 30 g
  ↓
timeout
  ↓
ERROR
```