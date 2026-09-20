# Tela de configuração

## 15. Tela de configuração

Arquivo:

```text
src/ui/screens/config_screen.c
```

A tela é responsável por configurar os parâmetros da dosagem antes da execução.

Apresenta:

```text
Configurar dosagem
```

e identifica o modo selecionado.

---

## 16. Configuração de quantidade fixa

No modo:

```text
CONFIG_MODE_FIXED_AMOUNT
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

## 17. Configuração de porções

No modo:

```text
CONFIG_MODE_PORTIONS
```

a interface apresenta o número de porções.

O valor inicial atual é:

```text
1
```

Os controles são:

```text
[-]    1    [+]
```

O incremento é:

```text
1
```

e o mínimo é:

```text
1
```

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
typedef struct {
    int target_grams;
    int portions;
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
    .target_grams = 100,
    .portions = 1
};
```

A tela de configuração altera esses valores através da interface.

Quando o usuário seleciona:

```text
Continuar
```

a configuração já está preparada para o início da operação.

---

## 21. Botão Continuar

O botão:

```text
Continuar
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
Continuar
   ↓
configuração pronta
   ↓
controller.start()
   ↓
Dosing
```