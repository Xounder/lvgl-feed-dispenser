# Configuração e dosagem normal

> _Voltar ao índice: [`../10-testing-strategy.md`](../10-testing-strategy.md)._

---

## 9. Teste do modo Massa

Selecionar:

```text
Massa
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

## 11. Teste do modo Valor (R$)

Selecionar:

```text
Valor (R$)
```

deve apresentar a configuração de valor monetário.

O valor inicial atual é:

```text
R$ 5,00
```

O incremento é:

```text
R$ 0,50
```

O mínimo é:

```text
R$ 0,50
```

Portanto:

```text
R$ 0,50 → [-] → continua R$ 0,50
```

A tela deve exibir a massa equivalente, calculada com o preço de referência:

```text
R$ 5,00 a R$ 12,00/kg = 416 g
```

A conversão pertence ao domínio:

```text
grams = (target_money_cents * 1000) / price_per_kg_cents
```

---

## 12. Teste de configuração antes da dosagem

Depois de configurar:

```text
target_grams        (modo Massa)
```

ou:

```text
target_money_cents + price_per_kg_cents   (modo Valor R$)
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
Modo: Massa
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
etapa rápida (faltando mais de 30 g)  +20 g por atualização
   ↓
etapa fina (faltando 30 g ou menos)   +2 g por atualização
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

Atualmente o crescimento depende da fase:

```text
etapa rápida: +20 g por atualização (faltando mais de 30 g)
etapa fina:   +2 g por atualização  (faltando 30 g ou menos)
```

Portanto, uma sequência aproximada para meta de 100 g é:

```text
0 → 20 → 40 → 60 → 80 → 82 → 84 → ... → 98 → 100
```

A interface deve acompanhar essa evolução e exibir a fase atual (Etapa rapida/Etapa fina).

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
Completed / Interrupted
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

Isso é garantido atualmente pela tara executada por:

```text
dosing_controller_new_dosing()
```

que também volta o controller para `IDLE`.

---

## 19. Teste de reset (tara)

Ao iniciar uma nova dosagem:

```text
dosing_controller_new_dosing()
    ↓
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