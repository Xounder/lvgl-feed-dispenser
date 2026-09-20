# Configuração e dosagem normal

> _Voltar ao índice: [`../10-testing-strategy.md`](../10-testing-strategy.md)._

---

## 9. Teste do modo de quantidade fixa

Selecionar:

```text
Quantidade fixa
```

deve levar para a configuração da quantidade em gramas.

A configuração inicial atual é:

```text
100 g
```

O valor deve poder ser alterado através dos controles:

```text
[-]   quantidade   [+]
```

---

## 10. Teste dos limites da quantidade

O valor mínimo atual é:

```text
10 g
```

Portanto:

```text
10 g → pressionar [-] → continua 10 g
```

O valor não deve ficar negativo ou chegar a zero através da interface atual.

O incremento atual é:

```text
10 g
```

Exemplo:

```text
100
 ↓ +
110
 ↓ +
120
```

---

## 11. Teste do modo de porções

Selecionar:

```text
Porcoes
```

deve apresentar a configuração de quantidade de porções.

O valor inicial atual é:

```text
1
```

O incremento é:

```text
1
```

O mínimo é:

```text
1
```

Portanto:

```text
1 → [-] → continua 1
```

---

## 12. Teste de configuração antes da dosagem

Depois de configurar:

```text
target_grams
```

ou:

```text
portions
```

o botão:

```text
Continuar
```

deve iniciar a etapa de dosagem.

A configuração escolhida deve ser a utilizada pelo controller.

---

## 13. Teste de dosagem normal

Este é o cenário principal do sistema.

### Pré-condições

Exemplo:

```text
Modo: quantidade fixa
Meta: 100 g
```

### Ação

Pressionar:

```text
Continuar
```

### Fluxo esperado

```text
Dosing
   ↓
peso = 0
   ↓
dispenser ativo
   ↓
peso aumenta
   ↓
peso >= 100 g
   ↓
dispenser para
   ↓
Completed
```

---

## 14. Critério de sucesso da dosagem

A regra atualmente implementada é:

```text
current_weight >= target_grams
```

Quando essa condição ocorre:

```text
dispenser.stop()
```

e:

```text
state = DOSING_STATE_COMPLETED
```

O teste deve confirmar os dois comportamentos.

Não basta a interface mostrar 100 g.

O dispenser também deve ter sido parado.

---

## 15. Teste de peso crescente

Durante a dosagem, o peso simulado deve aumentar.

Atualmente:

```text
+2 g
```

por atualização quando o dispenser está ativo.

Portanto, uma sequência aproximada pode ser:

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

A interface deve acompanhar essa evolução.

---

## 16. Teste de progress bar

A barra de progresso deve representar a relação entre:

```text
peso atual
```

e:

```text
peso alvo
```

Exemplo:

```text
Meta: 100 g
Atual: 50 g

Progresso ≈ 50%
```

Ao atingir a meta:

```text
Atual: 100 g
Progresso: 100%
```

---

## 17. Teste de conclusão

Depois de atingir a meta, deve ocorrer:

```text
Dosing
 ↓
Completed
```

A tela de conclusão deve apresentar a informação correspondente à dosagem realizada.

Também deve ser possível:

```text
Nova dosagem
```

e:

```text
Voltar ao inicio
```

---

## 18. Teste de nova dosagem

Depois de concluir uma dosagem:

```text
Completed
 ↓
Nova dosagem
 ↓
Mode
```

Deve ser possível iniciar uma nova operação.

O peso da nova dosagem deve começar novamente em:

```text
0 g
```

Isso é garantido atualmente pelo reset realizado no início da dosagem.

---

## 19. Teste de reset

Ao iniciar uma nova dosagem:

```text
simulated_weight_sensor.reset()
```

deve ser executado.

### Cenário

Primeira dosagem:

```text
100 g
```

Depois iniciar outra.

### Esperado

A nova dosagem começa em:

```text
0 g
```

e não em:

```text
100 g
```