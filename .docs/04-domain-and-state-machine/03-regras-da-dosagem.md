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
faltam > 30 g  → início na fase RÁPIDA (FAST)
faltam <= 30 g → início/transição para fase FINA (FINE)
```

Cada `tick` corresponde a uma atualização do controller (intervalo de 300 ms no simulador).

Formalmente:

```c
typedef enum {
    DOSING_PHASE_FAST,
    DOSING_PHASE_FINE
} DosingPhase;
```

O controller comanda `dispenser.start(DISPENSER_FLOW_FAST)` ou
`dispenser.start(DISPENSER_FLOW_FINE)`, inclusive ao iniciar com massa já
presente. No ESP32, esses perfis correspondem a aberturas distintas do servo;
no PC, os incrementos continuam sendo +20 g por tick rápido e +2 g por tick fino.
O peso real continua vindo exclusivamente da célula de carga/HX711.

Uma vez atingida a fase fina, o controller não retorna à rápida nessa
operação, mesmo se a leitura oscilar e a diferença voltar a ultrapassar 30 g.
Isso evita alternância de posições perto do limiar. Nova dosagem escolhe a
fase novamente. O limiar está em `FINE_THRESHOLD_GRAMS` no domínio;
os ângulos ficam no `board_config.h`, sem GPIOs ou graus no controlador.

Antes de abrir, `start()` verifica a leitura e o alvo: se a massa já alcança
a meta, fecha e conclui sem abrir. Alvo fora de 1–40000 g ou leitura fora de
0–40000 g impede início e resulta em `INTERRUPTED`. O limite máximo de 40 kg
é definido por `MAX_VALID_GRAMS` no controlador e `WEIGHT_MAX_GRAMS` no
driver real. A capacidade informada não substitui
calibração e validação da balança. Chamadas repetidas de
início durante `DOSING` são ignoradas.

A disponibilidade/frescura da leitura não é exposta pela interface atual do
sensor: o HX711 pode devolver o último valor válido. A validação de faixa
não detecta esse caso nem substitui timeout/supervisão de sensor.

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

A liberação manual usa `DISPENSER_FLOW_MANUAL`, disponível fora de `DOSING`.
Soltar um comando manual que não está ativo não interrompe o automático.
Durante a dosagem, nenhuma liberação manual é iniciada. Após cancelamento,
os ticks não retornam às fases automáticas nem reabrem o dispenser.

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
