# Simulador como ferramenta de testes, cenários e matriz

> _Voltar ao índice: [`../10-testing-strategy.md`](../10-testing-strategy.md)._

---

## 51. Simulador como ferramenta de testes

A implementação simulada deve evoluir além de:

```text
peso += 2
```

quando necessário.

Ela pode futuramente simular:

```text
peso normal
sensor parado
sensor instável
overshoot
dispenser travado
atraso
erro de leitura
```

Assim, situações difíceis de reproduzir no hardware podem ser testadas deterministicamente.

---

## 52. Cenário: sensor parado

Uma implementação futura poderia oferecer:

```text
Simulação:
sensor = sem progresso
```

Com:

```text
target = 100 g
```

espera-se:

```text
0 g
0 g
0 g
...
timeout
...
ERROR
```

O dispenser deve ser parado.

---

## 53. Cenário: overshoot

Outra simulação:

```text
target = 100 g
```

e o sensor produzir:

```text
98
100
105
```

O teste deve verificar qual política foi definida para o peso final.

Esse cenário será particularmente importante depois que o comportamento real do mecanismo for conhecido.

---

## 54. Cenário: sensor instável

Uma futura simulação pode produzir:

```text
98
101
99
102
100
```

Isso ajuda a verificar se o controller reage corretamente a pequenas oscilações.

Em hardware real, o peso provavelmente precisará de filtragem e/ou estabilização.

---

## 55. Cenário: dispenser parado

Simular:

```text
dispenser.start()
```

mas impedir que o peso aumente.

Resultado esperado futuramente:

```text
sem progresso
 ↓
timeout
 ↓
dispenser.stop()
 ↓
ERROR
```

---

## 56. Cenário: dispenser travado

O inverso também pode ser simulado:

```text
peso aumenta
```

mas:

```text
dispenser.stop()
```

não consegue efetivamente interromper o mecanismo.

No simulador isso pode ser representado como uma falha proposital da implementação.

No hardware real, esse tipo de problema exige uma estratégia de segurança apropriada.

---

## 57. Máquina de estados e testes

A máquina de estados deve orientar os testes.

A arquitetura planejada é:

```text
IDLE
 ↓
SELECT_MODE
 ↓
CONFIGURING
 ↓
DOSING
 ├──→ COMPLETED
 └──→ ERROR
```

O estado atual implementado ainda é simplificado:

```text
IDLE
DOSING
COMPLETED
```

Portanto, os testes devem distinguir:

```text
comportamento já implementado
```

de:

```text
comportamento planejado
```

---

## 58. Matriz de cenários

| Cenário               | Entrada            | Resultado esperado           | Estado futuro          |
| --------------------- | ------------------ | ---------------------------- | ---------------------- |
| Dosagem normal        | peso aumenta       | atingir meta e parar         | COMPLETED              |
| Interrupção           | usuário interrompe (Parar/Emergência) | parar dispenser              | INTERRUPTED            |
| Nova dosagem          | iniciar novamente  | peso resetado                | DOSING                 |
| Sensor parado         | peso não muda      | timeout                      | ERROR                  |
| Timeout global        | tempo excedido     | parar dispenser              | ERROR                  |
| Overshoot pequeno     | peso passa da meta | concluir conforme tolerância | COMPLETED              |
| Excesso grande        | peso muito acima   | registrar/tratar excesso     | ERROR/definição futura |
| Sensor inválido       | leitura inválida   | interromper com segurança    | ERROR                  |
| Configuração inválida | meta inválida      | rejeitar                     | CONFIGURING            |
| Dispenser falha       | sem alimentação    | detectar falta de progresso  | ERROR                  |
| Janela fechada        | encerramento       | hardware seguro              | IDLE/boot seguro       |
| Meta já atingida      | peso >= meta       | não dosar desnecessariamente | COMPLETED              |