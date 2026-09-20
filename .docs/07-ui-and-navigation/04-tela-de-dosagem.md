# Tela de dosagem

## 22. Tela de dosagem

Arquivo:

```text
src/ui/screens/dosing_screen.c
```

Essa é a tela operacional.

Ela acompanha uma dosagem em andamento.

Atualmente apresenta:

```text
Dosando
```

além de:

```text
objetivo
peso atual
barra de progresso
status
Cancelar
```

---

## 23. Objetivo

A tela apresenta o objetivo configurado.

Exemplo:

```text
Meta: 100 g
```

Esse valor vem da configuração da dosagem.

A tela não deve alterar o objetivo durante uma dosagem em andamento.

---

## 24. Peso atual

A tela mostra o valor retornado pelo domínio:

```text
dosing_controller_get_weight()
```

Exemplo:

```text
Peso atual: 42 g
```

O fluxo é:

```text
sensor
   ↓
controller
   ↓
get_weight()
   ↓
dosing screen
   ↓
label
```

A UI não lê diretamente o HX711.

A camada de hardware é descrita em [05-hardware-abstraction.md](../05-hardware-abstraction.md).

---

## 25. Barra de progresso

A barra de progresso é uma representação visual do avanço da dosagem.

Conceitualmente:

```text
progresso =
peso atual / peso objetivo
```

Por exemplo:

```text
objetivo = 100 g
peso = 50 g

progresso = 50%
```

A barra é uma visualização.

Ela não deve ser utilizada como fonte da verdade para o controller.

---

## 26. Fonte da verdade

A fonte da verdade é o domínio.

Não é:

```text
progress bar
```

nem:

```text
label
```

nem:

```text
screen state
```

O fluxo correto é:

```text
WeightSensor
     ↓
DosingController
     ↓
peso / estado
     ↓
UI
```

---

## 27. Timer da tela de dosagem

A tela de dosagem possui um timer periódico.

Atualmente ele é executado aproximadamente a cada:

```text
300 ms
```

O timer chama:

```c
dosing_controller_update();
```

Depois consulta:

```c
dosing_controller_get_weight();
dosing_controller_get_state();
```

e atualiza a interface.

---

## 28. Papel do timer

O timer permite que a UI acompanhe uma operação que continua acontecendo enquanto a tela permanece aberta.

Conceitualmente:

```text
Timer
  ↓
controller.update()
  ↓
estado/peso
  ↓
UI refresh
```

A UI não executa a lógica de dosagem diretamente.

Ela apenas dispara a atualização e apresenta o resultado.

---

## 29. Conclusão da dosagem

Quando:

```text
dosing_controller_get_state()
```

retorna:

```text
DOSING_STATE_COMPLETED
```

a tela:

1. remove o timer;
2. solicita a tela de conclusão.

O fluxo é:

```text
peso >= objetivo
       ↓
controller
       ↓
COMPLETED
       ↓
dosing screen detecta
       ↓
SCREEN_COMPLETED
```

---

## 30. Cancelamento

A tela de dosagem possui:

```text
Cancelar
```

Quando acionado:

```text
dosing_controller_cancel();
```

O controller:

```text
para o dispenser
```

e:

```text
retorna para IDLE
```

A UI então retorna para:

```text
SCREEN_HOME
```

---

## 31. Por que o botão Cancelar não controla o hardware diretamente

A tela não deve fazer algo como:

```text
servo_stop()
```

ou:

```text
simulated_dispenser.stop()
```

diretamente.

Ela solicita:

```text
dosing_controller_cancel()
```

O domínio então decide como o cancelamento deve ser executado.

Atualmente:

```text
UI
 ↓
controller.cancel()
 ↓
dispenser.stop()
 ↓
IDLE
```

Essa separação será importante quando o dispenser simulado for substituído pelo SG90 real.