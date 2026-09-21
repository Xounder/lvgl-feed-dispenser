# Tela de configuração

## 15. Tela de configuração

Arquivo:

```text
src/ui/screens/config_screen.c
```

A tela é responsável por configurar os parâmetros da dosagem antes da
execução e corresponde à **TELA 2** (`Aguardando - Definição de
Quantidade`) do mockup `.images/tela-aguardando.md`.

Estrutura:

```text
AGUARDANDO
Defina a quantidade

┌────────────────────┐  ┌──────────────────┐
│  MASSA (g)         │  │ VALOR (R$)       │
└────────────────────┘  └──────────────────┘

Quantidade desejada
  [- ]        500 g        [ + ]

Valores rapidos
┌────────┐ ┌────────┐ ┌────────┐
│ 100 g  │ │ 250 g  │ │ 500 g  │
└────────┘ └────────┘ └────────┘
┌────────┐ ┌────────┐ ┌────────┐
│ 750 g  │ │1.000 g │ │2.000 g │
└────────┘ └────────┘ └────────┘

▶ INICIAR DOSAGEM AUTOMATICA
VOLTAR
```

As abas `MASSA (g)` / `VALOR (R$)` alternam o modo:
`screen_manager_show_config(...)`. No modo Valor (R$), os valores rápidos
são `R$ 5,00 / 10,00 / 20,00 / 50,00 / 100,00 / 200,00` (centavos) e uma
linha adicional mostra:

```text
Equivale a X g
```

com o preço de referência do domínio.

---

## 16. Configuração do modo Massa

No modo:

```text
DOSING_MODE_GRAMS
```

a interface apresenta uma quantidade em gramas.

O valor inicial atual é:

```text
100 g
```

Os controles são:

```text
[-]    100 g    [+]
```

O incremento atual é:

```text
10 g
```

e o valor mínimo é:

```text
10 g
```

---

## 17. Configuração do modo Valor (R$)

No modo:

```text
DOSING_MODE_CURRENCY
```

a interface apresenta um valor monetário em reais (armazenado internamente em centavos).

O valor inicial atual é:

```text
R$ 5,00
```

Os controles são:

```text
[-]    R$ 5,00    [+]
```

O incremento atual é:

```text
R$ 0,50
```

e o valor mínimo é:

```text
R$ 0,50
```

Ao lado do valor é exibida a massa correspondente, calculada com o preço de referência:

```text
Equivale a 416 g   (com preço R$ 12,00/kg)
```

A conversão pertence ao domínio (ver modo Valor em [04-domain-and-state-machine.md](../04-domain-and-state-machine.md)).

---

## 18. Responsabilidade da configuração

A tela de configuração deve permitir ao usuário definir os parâmetros.

Ela não deve decidir:

```text
quando o dispenser deve parar
```

nem:

```text
como o peso é medido
```

Essas decisões pertencem ao domínio.

A UI apenas produz uma configuração.

---

## 19. `DosingConfig`

A configuração atual é representada por:

```c
typedef enum {
    DOSING_MODE_GRAMS,
    DOSING_MODE_CURRENCY
} DosingMode;

typedef struct {
    DosingMode mode;
    int target_grams;          /* modo massa (g) */
    int target_money_cents;    /* modo valor (R$ em centavos) */
    int price_per_kg_cents;    /* preco de referencia por kg (centavos) */
} DosingConfig;
```

Ela pertence ao domínio:

```text
src/domain/dosing_config.h
```

Isso é importante porque a configuração não é apenas um detalhe visual.

Ela representa dados necessários para a operação de dosagem.

Para a definição completa do domínio: [04-domain-and-state-machine.md](../04-domain-and-state-machine.md).

---

## 20. Relação entre UI e `DosingConfig`

O `Screen Manager` mantém atualmente uma configuração:

```c
static DosingConfig dosing_config = {
    .mode = DOSING_MODE_GRAMS,
    .target_grams = 100,
    .target_money_cents = 500,   /* R$ 5,00 */
    .price_per_kg_cents = 1200   /* R$ 12,00/kg */
};
```

A tela de configuração altera esses valores através da interface.

Quando o usuário seleciona:

```text
INICIAR DOSAGEM AUTOMATICA
```

a configuração já está preparada para o início da operação.

---

## 21. Botão INICIAR

O botão:

```text
INICIAR DOSAGEM AUTOMATICA
```

leva para:

```text
SCREEN_DOSING
```

Nesse momento, o `Screen Manager` inicia a operação:

```c
dosing_controller_start();
```

e depois carrega a tela de dosagem.

Conceitualmente:

```text
INICIAR DOSAGEM AUTOMATICA
   ↓
configuração pronta
   ↓
controller.start()
   ↓
Dosing
```