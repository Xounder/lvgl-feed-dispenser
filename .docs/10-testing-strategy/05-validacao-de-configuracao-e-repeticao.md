# Validação de configuração e repetição de dosagens

> _Voltar ao índice: [`../10-testing-strategy.md`](../10-testing-strategy.md)._

---

## 37. Teste de configuração inválida

A UI deve impedir ou rejeitar valores inválidos.

Exemplos:

```text
modo Massa:      0 g, -10 g
modo Valor (R$): R$ 0,00, preço de referência nulo ou negativo
```

No estado atual, os controles possuem limites mínimos.

Mesmo assim, o domínio futuramente deve validar os dados independentemente da UI.

---

## 38. Por que validar no domínio

A UI não deve ser a única barreira contra dados inválidos.

Conceitualmente:

```text
UI
 ↓
configuração
 ↓
Domain validation
 ↓
DosingController
```

Isso evita que outra origem de dados consiga iniciar uma operação inválida.

---

## 39. Teste de configuração extrema

Também devem ser testados valores muito altos.

Exemplo:

```text
10000 g
```

ou outro valor acima da capacidade física esperada.

O sistema deve possuir uma política para:

```text
valor máximo
capacidade máxima
tempo máximo
```

Esses limites deverão ser definidos conforme o hardware real.

---

## 40. Teste de meta já atingida

Um caso importante é:

```text
current_weight >= target
```

antes de ativar o dispenser.

O comportamento desejado deve ser:

```text
não iniciar alimentação desnecessariamente
```

Em uma implementação mais robusta:

```text
ler peso
 ↓
já atingiu meta?
 ├── sim → concluir
 └── não → iniciar dispenser
```

O comportamento exato deve ser consolidado conforme a evolução do controller.

---

## 41. Teste de valor muito pequeno

Para:

```text
target = 10 g
```

a dosagem deve:

```text
iniciar
 ↓
aumentar peso
 ↓
atingir 10 g
 ↓
parar
```

Esse cenário ajuda a verificar se a lógica não depende de metas grandes.

---

## 42. Teste de múltiplas dosagens

Executar várias operações consecutivas:

```text
Dosagem 1
 ↓
Completed
 ↓
Nova dosagem
 ↓
Dosagem 2
 ↓
Completed
 ↓
Nova dosagem
 ↓
Dosagem 3
```

O objetivo é detectar:

* estado residual;
* peso não resetado;
* timers duplicados;
* callbacks antigos;
* objetos de tela acumulados;
* comportamento diferente após a primeira execução.

---

## 43. Teste de repetição rápida

Também deve ser testado:

```text
Continuar
Continuar
Continuar
```

ou múltiplos eventos muito rápidos.

A aplicação deve evitar iniciar múltiplas dosagens simultaneamente.

Isso é particularmente importante porque o sistema possui timers associados à tela de dosagem.