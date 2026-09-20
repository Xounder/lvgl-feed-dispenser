## 05. UI, estados e controller

[voltar ao índice](../03-architecture-decisions.md)

---

### 1. Decisão: separar UI e regras de negócio

**Decisão**

A UI não deve ser responsável por implementar as regras da dosagem.

A UI:

```text
recebe interação
       ↓
solicita ação
       ↓
mostra resultado
```

O domínio:

```text
recebe ação
       ↓
aplica regras
       ↓
altera estado
       ↓
controla hardware através das abstrações
```

**Exemplo**

A tela não deve decidir:

```c
if (weight >= target) {
    stop_servo();
}
```

Ela deve consultar o domínio.

O controller é quem deve decidir:

```text
peso atingiu objetivo?
       ↓
      sim
       ↓
parar dispenser
       ↓
COMPLETED
```

A UI apenas representa o resultado.

Status: **Atual**.

Mais detalhes de UI: [07-ui-and-navigation.md](../07-ui-and-navigation.md).

---

### 2. Decisão: utilizar um DosingController

**Decisão**

A lógica central da dosagem foi concentrada em um componente chamado:

```text
DosingController
```

**Motivo**

Sem um controller central, seria fácil distribuir a lógica entre:

```text
home_screen
config_screen
dosing_screen
callbacks
timers
hardware
```

Isso dificultaria entender o fluxo completo.

O controller fornece um ponto central para o processo:

```text
start
update
cancel
get_weight
get_state
```

Status: **Atual**.

Responsabilidades do controller: [04-domain-and-state-machine.md](../04-domain-and-state-machine.md).

---

### 3. Decisão: utilizar estados explícitos

**Decisão**

O processo de dosagem deve ser representado por estados.

Atualmente existe:

```text
IDLE
DOSING
COMPLETED
```

A arquitetura prevista poderá evoluir para estados adicionais, como:

```text
SELECT_MODE
CONFIGURING
ERROR
```

**Motivo**

Um sistema físico possui comportamento temporal.

Ele não está simplesmente:

```text
ligado/desligado
```

Pode estar:

```text
aguardando
configurando
dosando
finalizando
concluído
com erro
cancelado
```

Representar isso explicitamente reduz ambiguidades no comportamento.

Status: **Atual** (evolução possível: `SELECT_MODE`, `CONFIGURING`, `ERROR`). Máquina de estados: [04-domain-and-state-machine.md](../04-domain-and-state-machine.md).

---

### 4. Decisão: manter configuração separada

A configuração da dosagem é representada por:

```text
DosingConfig
```

Isso evita que valores como:

```text
target_grams
portions
```

fiquem espalhados entre telas e callbacks.

A UI pode editar a configuração.

O controller utiliza essa configuração para executar a dosagem.

Status: **Atual**.

Uso da configuração na UI: [07-ui-and-navigation.md](../07-ui-and-navigation.md).