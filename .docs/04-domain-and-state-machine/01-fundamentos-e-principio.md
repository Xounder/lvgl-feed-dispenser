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
                         │     IDLE      │◄──────────────┐
                         └───┬───────┬───┘                │
                             │   manual_release           │
                          iniciar ▼                       │
                             │   liberação manual         │
                             │   (permanece em IDLE)      │
                             ▼                            │
               ┌─────────────────────────┐                │
               │     SELECT_MODE         │                │
               └────────────┬────────────┘                │
                            │                             │
                     selecionar modo                      │
                            │                             │
                            ▼                             │
               ┌─────────────────────────┐                │
               │      CONFIGURING        │                │
               └────────────┬────────────┘                │
                            │                             │
                         iniciar                          │
                            │                             │
                            ▼                             │
               ┌─────────────────────────┐                │
               │        DOSING           │                │
               └──┬──────────┬────────┬──┘                │
                  │          │        │                   │
            meta atingida  cancelar/  erro                 │
                           emergência                     │
                  │          │        │                   │
                  ▼          ▼        ▼                   │
              COMPLETED   INTERRUPTED ERROR                │
                  │          │        │                   │
                  └──────────┴────────┴── finalizar ────────┘
                       │          │        │
                  nova dosagem  nova dosagem  reconhecer
                      (tara)      (tara)
                       └──────────┴──────────┘
                                    │
                                    ▼
                                 IDLE
```

Essa é a direção arquitetural desejada.

`SELECT_MODE`, `CONFIGURING` e `ERROR` fazem parte da evolução prevista.

A implementação atual possui um subconjunto simplificado desses estados (ver seção 4).

---

## 4. Estados atuais

Atualmente o controller possui:

```c
typedef enum {
    DOSING_STATE_IDLE,
    DOSING_STATE_DOSING,
    DOSING_STATE_COMPLETED,
    DOSING_STATE_INTERRUPTED
} DosingState;
```

Portanto:

```text
IDLE
DOSING
COMPLETED
INTERRUPTED
```

já existem na implementação.

O controller também possui as fases da dosagem:

```c
typedef enum {
    DOSING_PHASE_FAST,
    DOSING_PHASE_FINE
} DosingPhase;
```

Estados como:

```text
SELECT_MODE
CONFIGURING
ERROR
```

fazem parte da evolução prevista da máquina de estados.