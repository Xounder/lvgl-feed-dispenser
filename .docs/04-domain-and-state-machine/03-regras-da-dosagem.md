## 12. Regra principal da dosagem

_Voltar ao índice: [`../04-domain-and-state-machine.md`](../04-domain-and-state-machine.md)._

---

A regra fundamental é:

```text
Se peso atual >= alvo efetivo (em gramas):
    parar dispenser
    concluir dosagem
```

Formalmente:

```text
current_weight >= target_grams_efetivo
```

resulta em:

```text
dispenser.stop()
state = COMPLETED
```

O alvo efetivo em gramas é obtido pelo controller: diretamente de `target_grams` no modo Massa, ou pela conversão do modo Valor:

```text
target_grams = (target_money_cents * 1000) / price_per_kg_cents
```

Essa é uma das regras centrais do domínio.

---

## 13. Continuação da dosagem

Enquanto:

```text
current_weight < target_grams_efetivo
```

o processo continua.

A dosagem é dividida em duas etapas controladas por fases:

```text
faltam > 30 g  → fase RÁPIDA (FAST) → +20 g por tick
faltam <= 30 g → fase FINA (FINE)   → +2 g por tick
```

Cada `tick` corresponde a uma atualização do controller (intervalo de 300 ms no simulador).

Formalmente:

```c
typedef enum {
    DOSING_PHASE_FAST,
    DOSING_PHASE_FINE
} DosingPhase;
```

Isso é apenas um modelo de simulação.

No hardware real:

```text
peso
```

será determinado pela célula de carga através do HX711.

---

## 14. Estado `COMPLETED`

### Significado

A quantidade alvo foi atingida e o processo foi encerrado normalmente.

Ao entrar nesse estado:

```text
dispenser = parado
dosagem = concluída
```

A UI pode apresentar:

```text
Dosagem concluída
```

---

## 15. Regra de conclusão

Uma dosagem só deve ser considerada concluída quando:

```text
peso >= objetivo
```

e o processo de liberação for interrompido.

Portanto:

```text
DOSING
   │
   │ peso >= objetivo
   ▼
parar dispenser
   │
   ▼
COMPLETED
```

---

## 16. Interrupção da dosagem

A dosagem pode ser interrompida por dois eventos prioritários:

```text
CANCEL         — botão "Parar" na tela
EMERGENCY_STOP — botão de emergência físico
```

Fluxo:

```text
DOSING
   │
   │ cancelar / emergência
   ▼
parar dispenser
   │
   ▼
INTERRUPTED
```

O dispenser nunca deve permanecer ativo após uma interrupção.

A regra é:

```text
interromper (CANCEL / EMERGENCY_STOP)
    ↓
stop dispenser
    ↓
state = INTERRUPTED
```

### Massa parcial preservada

A interrupção preserva a massa parcialmente liberada: **não** é feita tara/reset do sensor nesse momento.

Para iniciar uma nova dosagem, o usuário utiliza `new_dosing`:

```text
INTERRUPTED
    │
    │ new_dosing
    ▼
tara/reset do sensor
    │
    ▼
IDLE
```

A partir de `IDLE`, uma nova dosagem pode ser iniciada (o ciclo volta ao fluxo normal).

---

## 17. Estado `ERROR`

O estado de erro ainda não está implementado no controller atual, mas faz parte da máquina de estados planejada.

Ele representa uma condição na qual o processo não pode continuar de maneira segura ou confiável.

Exemplos:

```text
sensor indisponível
peso inválido
peso não aumenta
timeout
dispenser não responde
falha de hardware
```

---

## 18. Entrada em `ERROR`

Uma condição de erro deve interromper o processo de dosagem.

Fluxo esperado:

```text
DOSING
   │
   │ erro detectado
   ▼
parar dispenser
   │
   ▼
ERROR
```

A regra de segurança é importante:

> **Ao detectar uma falha durante a dosagem, o dispenser deve ser colocado em estado seguro antes de informar o erro à UI.**

---

## 19. Saída de `ERROR`

A saída dependerá do tipo de erro.

No modelo inicial, a estratégia mais simples é:

```text
ERROR
  │
  │ reconhecer / voltar
  ▼
IDLE
```

Em erros recuperáveis, futuramente poderá existir:

```text
ERROR
  │
  │ tentar novamente
  ▼
CONFIGURING
```

Essa distinção deve ser criada somente quando houver requisitos concretos.

---

## 20. Máquina de estados detalhada

A máquina pode ser representada assim:

```text
                         ┌──────────────┐
                         │     IDLE     │
                         └──────┬───────┘
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
                         configuração válida
                                │
                              iniciar
                                │
                                ▼
                    ┌────────────────────────┐
                    │        DOSING           │
                    └─────┬──────┬───────┬───┘
                          │      │       │
                    cancelar   erro    objetivo
                          │      │     atingido
                          │      │       │
                          ▼      ▼       ▼
                        IDLE   ERROR  COMPLETED
                                  │       │
                                  │       │
                             reconhecer  nova dosagem
                                  │       │
                                  ▼       ▼
                                IDLE  SELECT_MODE
```