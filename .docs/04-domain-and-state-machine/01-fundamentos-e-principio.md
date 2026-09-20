## 1. Objetivo

_Voltar ao índice: [`../04-domain-and-state-machine.md`](../04-domain-and-state-machine.md)._

---

Este documento define o comportamento do domínio do dosador de ração e sua máquina de estados.

Ele descreve:

* estados da aplicação;
* significado de cada estado;
* transições;
* eventos que provocam transições;
* regras da dosagem;
* configuração;
* cancelamento;
* conclusão;
* erros;
* comportamento esperado do controller.

O objetivo é fornecer uma referência para a implementação atual e para futuras evoluções do sistema.

---

## 2. Princípio fundamental

O estado do domínio representa o **processo real de dosagem**, e não simplesmente a tela que está sendo exibida.

Portanto:

```text
Estado do domínio
        ≠
Tela do LVGL
```

Existe atualmente uma forte relação entre eles, mas são conceitos diferentes.

Por exemplo:

```text
DosingState = DOSING
```

significa:

> O sistema está executando uma dosagem.

Enquanto:

```text
SCREEN_DOSING
```

significa:

> A interface está apresentando a tela de dosagem.

Essa distinção deve ser preservada.

---

## 3. Máquina de estados geral

A máquina de estados desejada pode ser representada como:

```text
                         ┌───────────────┐
                         │     IDLE      │
                         └───────┬───────┘
                                 │
                              iniciar
                                 │
                                 ▼
                    ┌────────────────────────┐
                    │     SELECT_MODE        │
                    └───────────┬────────────┘
                                │
                         selecionar modo
                                │
                                ▼
                    ┌────────────────────────┐
                    │      CONFIGURING        │
                    └───────────┬────────────┘
                                │
                             iniciar
                                │
                                ▼
                    ┌────────────────────────┐
                    │        DOSING           │
                    └──────┬─────────┬───────┘
                           │         │
                     cancelar       erro
                           │         │
                           ▼         ▼
                        IDLE       ERROR
                           ▲         │
                           │         │
                           └─────────┘
                             finalizar

                    DOSING
                       │
                 meta atingida
                       │
                       ▼
                  COMPLETED
                       │
                 nova dosagem
                       │
                       ▼
                    SELECT_MODE
```

Essa é a direção arquitetural desejada.

A implementação atual possui um subconjunto simplificado desses estados.

---

## 4. Estados atuais

Atualmente o controller possui:

```c
typedef enum {
    DOSING_STATE_IDLE,
    DOSING_STATE_DOSING,
    DOSING_STATE_COMPLETED
} DosingState;
```

Portanto:

```text
IDLE
DOSING
COMPLETED
```

já existem na implementação.

Estados como:

```text
SELECT_MODE
CONFIGURING
ERROR
```

fazem parte da evolução prevista da máquina de estados.