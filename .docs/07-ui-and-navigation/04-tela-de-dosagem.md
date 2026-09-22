# Tela de dosagem

## 22. Tela de dosagem

Arquivo:

```text
src/ui/screens/dosing_screen.c
```

Essa é a tela operacional.

Ela acompanha uma dosagem em andamento e corresponde à **TELA 2**
(`Dosando`) do mockup `.images/tela-dosando.md`.

Atualmente apresenta:

```text
DOSANDO

Massa atual
┌────────────────────────────────────────┐
│ ████████████░░░░░░░░░░░░░░   52%      │
└────────────────────────────────────────┘
Meta: 100 g
Etapa rapida: vazao alta

■ INTERROMPER DOSAGEM

┌──────────────────────────────────────┐
│ LIBERACAO MANUAL (desabilitado)      │
└──────────────────────────────────────┘
```

A tela **não possui** botão separado de emergência: conforme o mockup,
há apenas o comando **INTERROMPER DOSAGEM** (RS11). O botão físico de
emergência é tratado no alvo embarcado (ver [05-botoes-e-indicadores.md](../08-target-hardware/05-botoes-e-indicadores.md)).

Como nas demais telas, o card desabilitado
`LIBERACAO MANUAL` fica na **parte inferior da tela**,
logo acima da barra de navegação.

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

## 29a. Fases rápidas e fina na tela

A tela exibe a fase atual da dosagem:

```text
Etapa rapida: vazao alta
Etapa fina: vazao reduzida
```

A fase é consultada no domínio:

```c
dosing_controller_get_phase();
```

Isso permite ao usuário observar o controle em duas etapas (abertura maior quando longe do alvo e abertura reduzida quando próximo — RP03/RS09).

---

## 30. Interrupção (INTERROMPER DOSAGEM)

A tela de dosagem possui um comando de interrupção na tela:

```text
[ INTERROMPER DOSAGEM ]   (comando na tela — RS11)
```

Quando acionado:

```text
dosing_controller_cancel();
```

O controller:

```text
para o dispenser
preserva a massa parcial
estado = INTERRUPTED
```

A UI então solicita a tela de interrupção:

```text
SCREEN_INTERRUPTED
```

O comando tem prioridade sobre o controle automático (RS12). Durante a
dosagem a **liberação manual é bloqueada** (RS15), o que é refletido na
tela pelo card desabilitado `LIBERACAO MANUAL`.

---

## 31. Por que os botões de interrupção não controlam o hardware diretamente

A tela não deve fazer algo como:

```text
servo_stop()
```

ou:

```text
dispenser.stop()
```

diretamente.

Ela solicita:

```text
dosing_controller_cancel()
```

O domínio então decide como a interrupção deve ser executada.

Atualmente:

```text
UI
 ↓
controller.cancel()
 ↓
dispenser.stop()
 ↓
INTERRUPTED (massa parcial preservada)
```

Essa separação será importante quando o dispenser simulado for substituído pelo SG90 real.