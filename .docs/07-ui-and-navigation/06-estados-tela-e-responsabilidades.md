# Estados de tela e responsabilidades

## 36. Estados de tela versus estados do domínio

É importante distinguir:

```text
Screen
```

de:

```text
DosingState
```

A enumeração da UI:

```c
SCREEN_HOME
SCREEN_MODE
SCREEN_CONFIG
SCREEN_DOSING
SCREEN_COMPLETED
SCREEN_INTERRUPTED
```

representa:

> qual tela está sendo apresentada.

Já o domínio possui:

```c
DOSING_STATE_IDLE
DOSING_STATE_DOSING
DOSING_STATE_COMPLETED
DOSING_STATE_INTERRUPTED
```

representando:

> em qual estado operacional a dosagem está.

São conceitos diferentes.

A máquina de estados do domínio é definida em [04-domain-and-state-machine.md](../04-domain-and-state-machine.md).

---

## 37. Por que não usar uma única enumeração

Seria tentador criar algo como:

```text
HOME
MODE
CONFIG
DOSING
COMPLETED
```

e utilizar isso como estado geral do sistema.

Isso mistura:

```text
navegação
```

com:

```text
comportamento de negócio
```

Uma tela pode mudar sem necessariamente mudar a regra de negócio.

A arquitetura deve preservar essa distinção.

---

## 38. Estado planejado versus telas

A máquina de estados desejada no domínio é:

```text
IDLE
  ↓
SELECT_MODE
  ↓
CONFIGURING
  ↓
DOSING
  ├── COMPLETED
  └── INTERRUPTED
```

Atualmente o domínio implementa uma versão simplificada:

```text
IDLE → DOSING → COMPLETED
            └──→ INTERRUPTED
```

com possibilidade futura de:

```text
DOSING
  ↓
ERROR
```

As telas são uma representação possível desses estados, mas não precisam ser uma cópia literal deles.

Por exemplo:

```text
SELECT_MODE
```

pode ser representado pela:

```text
SCREEN_MODE
```

e:

```text
CONFIGURING
```

pela:

```text
SCREEN_CONFIG
```

Mas a relação deve ser conceitual, não uma obrigação estrutural.

---

## 39. Regra de responsabilidade

A divisão desejada é:

### UI

```text
mostrar
receber interação
solicitar ação
```

### Screen Manager

```text
coordenar navegação
criar/carregar telas
```

### Domain

```text
decidir comportamento
validar regras
controlar ciclo da dosagem
```

### Hardware

```text
medir
acionar
```

---

## 40. Exemplo: usuário inicia dosagem

O usuário pressiona:

```text
Continuar
```

A sequência é:

```text
ConfigScreen
    ↓
screen_manager_show(SCREEN_DOSING)
    ↓
DosingController.start()
    ↓
WeightSensor.reset()
    ↓
Dispenser.start()
    ↓
DosingScreen
```

A UI não decide como o dispenser inicia.

---

## 41. Exemplo: peso atingiu objetivo

Durante a dosagem:

```text
DosingScreen
    ↓
controller.update()
    ↓
WeightSensor.read_grams()
    ↓
peso >= objetivo
    ↓
Dispenser.stop()
    ↓
state = COMPLETED
    ↓
DosingScreen detecta
    ↓
CompletedScreen
```

A UI apenas apresenta o resultado.

---

## 42. Exemplo: interrupção

```text
Usuário
   ↓
Parar / Emergencia
   ↓
DosingScreen
   ↓
dosing_controller_cancel()
   ↓
Dispenser.stop()
   ↓
state = INTERRUPTED (massa parcial preservada)
   ↓
InterruptedScreen
```

---

## 43. Comunicação da UI com o domínio

Atualmente, a comunicação é baseada principalmente em funções do controller:

```c
dosing_controller_start();
dosing_controller_update();
dosing_controller_cancel();
dosing_controller_get_weight();
dosing_controller_get_state();
```

Isso mantém a UI relativamente simples.

---

## 44. O que a UI não deve fazer

A UI não deve:

* ler HX711 diretamente;
* controlar GPIO;
* controlar PWM;
* mover SG90 diretamente;
* calcular regras de parada;
* decidir quando a dosagem terminou;
* implementar timeout de hardware;
* determinar se o sensor está funcionando;
* decidir como tratar uma falha física.

Essas responsabilidades pertencem ao domínio ou à camada de hardware.

---

## 45. O que a UI pode fazer

A UI pode:

* apresentar peso;
* apresentar objetivo;
* apresentar progresso;
* mostrar mensagens;
* receber comandos;
* alterar valores de configuração;
* solicitar início;
* solicitar cancelamento;
* solicitar nova dosagem;
* navegar entre telas.

---

## 46. Configuração e validação

A UI atualmente aplica restrições simples, como:

```text
modo Massa: mínimo = 10 g, incremento = 10 g
modo Valor (R$): mínimo = R$ 0,50, incremento = R$ 0,50
```

Essas regras ajudam a impedir entradas obviamente inválidas.

Entretanto, validações importantes do domínio não devem depender somente da UI.

Por exemplo:

```text
target <= 0
```

deve ser considerado inválido pelo domínio caso possa chegar até ele por outro caminho.

A UI é uma primeira barreira de entrada, não a única fonte de validação.