## 12. Regra principal da dosagem

_Voltar ao índice: [`../04-domain-and-state-machine.md`](../04-domain-and-state-machine.md)._

---

A regra fundamental é:

```text
Se peso atual >= peso alvo:
    parar dispenser
    concluir dosagem
```

Formalmente:

```text
current_weight >= target_grams
```

resulta em:

```text
dispenser.stop()
state = COMPLETED
```

Essa é uma das regras centrais do domínio.

---

## 13. Continuação da dosagem

Enquanto:

```text
current_weight < target_grams
```

o processo continua.

No simulador atual, quando o dispenser está ativo:

```text
peso += 2 g
```

a cada atualização do controller.

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

## 16. Cancelamento

O usuário pode cancelar uma dosagem em andamento.

Fluxo:

```text
DOSING
   │
   │ cancelar
   ▼
parar dispenser
   │
   ▼
IDLE
```

O dispenser nunca deve permanecer ativo após um cancelamento.

A regra é:

```text
cancelar
    ↓
stop dispenser
    ↓
state = IDLE
```

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