## 21. Tabela de estados

_Voltar ao índice: [`../04-domain-and-state-machine.md`](../04-domain-and-state-machine.md)._

---

| Estado        | Significado         | Dispenser     | Sensor           | Próximos estados                  |
| ------------- | ------------------- | ------------- | ---------------- | --------------------------------- |
| `IDLE`        | Sistema parado      | parado        | inativo          | `SELECT_MODE`                     |
| `SELECT_MODE` | Escolha do modo     | parado        | inativo          | `CONFIGURING`                     |
| `CONFIGURING` | Configuração        | parado        | inativo          | `DOSING`                          |
| `DOSING`      | Dosagem em execução | ativo/parando | monitorado       | `COMPLETED`, `INTERRUPTED`, `ERROR` |
| `COMPLETED`   | Dosagem concluída   | parado        | leitura final    | `SELECT_MODE`, `IDLE`             |
| `INTERRUPTED` | Interrompida (Parar/Emergência) | parado | massa parcial preservada | `SELECT_MODE`, `IDLE` |
| `ERROR`       | Falha no processo   | parado        | depende da falha | `IDLE`, futuramente `CONFIGURING` |

---

## 22. Eventos

As transições da máquina de estados são provocadas por eventos ou condições.

Principais eventos:

```text
START
MODE_SELECTED
CONFIG_CONFIRMED
INTERRUPT
TARGET_REACHED
ERROR_DETECTED
ERROR_ACKNOWLEDGED
NEW_DOSING
MANUAL_RELEASE_START
MANUAL_RELEASE_STOP
```

Nem todos estão implementados como eventos explícitos no código atual.

Alguns são atualmente representados diretamente por chamadas de funções ou callbacks.

---

## 23. Relação entre eventos e estados

| Evento               | Estado atual  | Resultado         |
| -------------------- | ------------- | ----------------- |
| `START`              | `IDLE`        | `SELECT_MODE`     |
| `MODE_SELECTED`      | `SELECT_MODE` | `CONFIGURING`     |
| `CONFIG_CONFIRMED`   | `CONFIGURING` | `DOSING`          |
| `INTERRUPT`          | `DOSING`      | `INTERRUPTED`     |
| `TARGET_REACHED`     | `DOSING`      | `COMPLETED`       |
| `ERROR_DETECTED`     | `DOSING`      | `ERROR`           |
| `ERROR_ACKNOWLEDGED` | `ERROR`       | `IDLE`            |
| `NEW_DOSING`         | `COMPLETED`/`INTERRUPTED` | `SELECT_MODE` (com tara) |

A interrupção (`INTERRUPT`) pode ser disparada por **Parar** (comando) ou **Emergencia** (botão físico), com prioridade sobre o controle automático.

---

## 24. Regras invariantes

Algumas condições devem permanecer verdadeiras independentemente da implementação.

### Regra 1 — dispenser parado fora da dosagem

Fora de `DOSING`, o dispenser deve estar parado.

```text
IDLE       → parado
SELECT_MODE → parado
CONFIGURING → parado
COMPLETED  → parado
ERROR      → parado
```

---

### Regra 2 — nunca concluir com peso abaixo do objetivo

Uma dosagem normal não deve entrar em:

```text
COMPLETED
```

se:

```text
current_weight < target_grams
```

---

### Regra 3 — interromper sempre para o dispenser

Ao interromper:

```text
INTERRUPT
  ↓
stop dispenser
  ↓
INTERRUPTED (massa parcial preservada)
```

A interrupção é uma ação de prioridade: independe do estado automático da dosagem.

---

### Regra 4 — erro deve levar o sistema a um estado seguro

Ao detectar uma falha durante a dosagem:

```text
erro
 ↓
parar dispenser
 ↓
ERROR
```

---

### Regra 5 — iniciar uma nova dosagem deve resetar o processo

Uma nova dosagem não deve herdar o peso da dosagem anterior.

O processo deve começar com uma nova referência de peso.

No simulador atual:

```text
dosing_controller_new_dosing()
    ↓
simulated_weight_sensor.reset()   (tara)
    ↓
IDLE
```

No hardware real, isso deverá corresponder à estratégia de tara/referência adotada (ver [`../05-hardware-abstraction.md`](../05-hardware-abstraction.md)).

---

### Regra 6 — liberação manual é exclusiva do estado parado

A liberação manual (RS14/RS16) só é permitida em `IDLE`:

```text
MANUAL_RELEASE_START (IDLE)
    ↓
dispenser liberando manualmente (+5 g por tick)
    ↓
MANUAL_RELEASE_STOP
```

Durante a dosagem automática, a liberação manual fica bloqueada (RS15), evitando conflito com o controle do servo.

---

## 25. Configuração versus execução

Uma distinção importante:

```text
CONFIGURING
```

é responsável por definir:

```text
o que fazer
```

enquanto:

```text
DOSING
```

é responsável por:

```text
executar o que foi definido
```

Portanto, durante a dosagem, a configuração não deve ser alterada arbitrariamente.

Exemplo:

```text
target = 100 g

DOSING
  ↓
peso = 45 g
```

O usuário não deve simplesmente alterar o alvo para:

```text
500 g
```

sem uma regra explícita que permita isso.

---

## 26. Snapshot da configuração

Ao iniciar uma dosagem, o controller deve trabalhar com uma configuração válida.

Conceitualmente:

```text
CONFIGURING
     ↓
validar
     ↓
iniciar
     ↓
DOSING
```

A configuração utilizada durante o processo deve permanecer estável.

Isso evita que alterações da UI provoquem mudanças inesperadas no processo físico.

---

## 27. Validação da configuração

Antes de iniciar a dosagem, a configuração deve ser validada.

Exemplos:

```text
modo Massa:      target_grams > 0
modo Valor (R$): target_money_cents > 0 e price_per_kg_cents > 0
```

Dependendo do modo, apenas os campos relevantes precisam ser considerados.

Exemplo:

```text
DOSING_MODE_GRAMS
    → target_grams obrigatório
```

```text
DOSING_MODE_CURRENCY
    → target_money_cents e price_per_kg_cents obrigatórios
```

No modo Valor (R$), o alvo em gramas é derivado no domínio:

```text
grams = (target_money_cents * 1000) / price_per_kg_cents
```