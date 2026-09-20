## 04. Fluxos, telas e atualização

[voltar ao índice](../02-architecture.md)

---

### 1. Fluxo de uma dosagem

Um ciclo típico funciona conceitualmente assim:

```text
1. Usuário configura dosagem
              │
              ▼
2. UI atualiza DosingConfig
              │
              ▼
3. Usuário pressiona "Continuar"
              │
              ▼
4. UI solicita início
              │
              ▼
5. DosingController inicia
              │
       ┌──────┴───────┐
       ▼              ▼
 reset sensor     start dispenser
       │              │
       └──────┬───────┘
              ▼
6. Controller entra em DOSING
              │
              ▼
7. Controller lê peso
              │
              ▼
8. Verifica objetivo
              │
       ┌──────┴──────┐
       │             │
   não atingiu    atingiu
       │             │
       ▼             ▼
  continua        stop dispenser
       │             │
       └──────┐      ▼
              │   COMPLETED
              ▼
          nova leitura
```

As etapas desta máquina correspondem aos estados documentados em [04-domain-and-state-machine.md](../04-domain-and-state-machine.md).

---

### 2. Fluxo entre UI e Controller

A UI não deve implementar o processo inteiro.

Por exemplo, a tela de dosagem pode executar:

```text
timer
  ↓
dosing_controller_update()
  ↓
dosing_controller_get_weight()
  ↓
atualiza label/progress bar
```

A decisão:

```text
"o peso atingiu a quantidade desejada?"
```

é responsabilidade do controller.

A decisão:

```text
"qual texto mostrar?"
```

é responsabilidade da UI.

O papel do timer e da tela de dosagem é detalhado em [07-ui-and-navigation.md](../07-ui-and-navigation.md).

---

### 3. Fluxo de atualização

No simulador atual, a aplicação possui um loop principal:

```text
main
 │
 ├── lv_timer_handler()
 │
 └── aguarda próximo ciclo
```

O LVGL gerencia os timers da aplicação.

A tela de dosagem possui um timer periódico que solicita atualização do processo:

```text
LVGL Timer
    ↓
dosing_screen_update()
    ↓
dosing_controller_update()
    ↓
read sensor
    ↓
control dispenser
    ↓
get state
    ↓
update UI
```

Esse mecanismo é adequado para o simulador atual.

A estratégia exata de temporização poderá mudar no ESP32 conforme os requisitos de tempo real e os drivers utilizados.

A relação entre tempo e atualização da UI é tratada em [06-simulation-strategy.md](../06-simulation-strategy.md).

---

### 4. Gerenciamento de telas

O `screen_manager` centraliza a troca de telas.

Fluxo conceitual:

```text
HOME
 │
 └──→ MODE
       │
       ├──→ CONFIG
       │      │
       │      └──→ DOSING
       │             │
       │             └──→ COMPLETED
       │
       └──→ HOME
```

A existência de um `screen_manager` evita que cada tela precise conhecer diretamente todas as outras telas.

Por exemplo:

```text
home_screen
     ↓
screen_manager
     ↓
mode_screen
```

em vez de:

```text
home_screen
     ↓
cria diretamente mode_screen
```

A navegação completa entre telas está em [07-ui-and-navigation.md](../07-ui-and-navigation.md).

---

### 5. Relação entre estado e tela

Estado de domínio e tela são conceitos diferentes.

Por exemplo:

```text
DOSING_STATE_DOSING
```

representa o estado do processo.

Já:

```text
SCREEN_DOSING
```

representa uma tela.

Atualmente existe uma correspondência forte entre ambos, mas isso não deve ser considerado uma regra arquitetural absoluta.

No futuro, um estado poderá existir sem uma tela própria.

Exemplo:

```text
DOSING
  ↓
ERROR
```

Uma tela de erro poderia apresentar diferentes tipos de falha sem que cada erro precise necessariamente virar um novo estado de tela.

A distinção entre estados de tela e estados de domínio é aprofundada em [07-ui-and-navigation.md](../07-ui-and-navigation.md) e em [04-domain-and-state-machine.md](../04-domain-and-state-machine.md).