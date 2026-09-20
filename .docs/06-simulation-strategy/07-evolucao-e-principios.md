# Evolução e princípios

## 45. Evolução incremental

A estratégia recomendada é:

```text
VERSÃO 1
peso +2 g/update
        ↓
VERSÃO 2
taxa baseada em tempo
        ↓
VERSÃO 3
overshoot
        ↓
VERSÃO 4
ruído
        ↓
VERSÃO 5
falhas
        ↓
VERSÃO 6
cenários configuráveis
        ↓
VERSÃO 7
testes automatizados
```

Não é necessário implementar tudo antes de continuar o projeto.

---

## 46. Critério para aumentar a complexidade

A simulação deve ficar mais complexa quando existir uma necessidade concreta.

Exemplos:

```text
Problema:
timer influencia resultado

Solução:
simulação baseada em tempo
```

```text
Problema:
controller não foi testado com overshoot

Solução:
adicionar overshoot
```

```text
Problema:
não existe teste de sensor travado

Solução:
adicionar falha configurável
```

Evitar:

```text
adicionar física complexa
```

sem um comportamento que precise ser validado.

---

## 47. Princípio de determinismo

Sempre que possível, os testes devem utilizar uma simulação determinística.

Isso significa:

```text
mesmas entradas
+
mesmo cenário
=
mesmo resultado
```

A aleatoriedade deve ser usada como ferramenta adicional, não como requisito para executar o sistema normalmente.

---

## 48. Princípio de isolamento

A simulação deve permanecer isolada das demais camadas.

Idealmente:

```text
UI
 ↓
Controller
 ↓
Hardware abstraction
 ↓
Simulation
```

A simulação não deve conhecer:

```text
LVGL
SDL2
widgets
screens
labels
progress bars
```

---

## 49. Princípio de substituição

Uma implementação simulada deve representar o mesmo contrato esperado da implementação real.

Por exemplo:

```text
                 WeightSensor
                     │
          ┌──────────┴──────────┐
          ▼                     ▼
SimulatedWeightSensor      HX711WeightSensor
```

Ambas devem oferecer ao domínio uma forma coerente de obter o peso.

O mesmo vale para:

```text
Dispenser
```

com:

```text
SimulatedDispenser
```

e:

```text
ServoDispenser
```

---

## 50. Estado atual versus objetivo futuro

### Atualmente

O simulador possui:

```text
✓ peso simulado
✓ dispenser simulado
✓ incremento de peso
✓ start
✓ stop
✓ cancelamento
✓ conclusão
✓ reset
```

### Ainda planejado

```text
○ taxa baseada em tempo
○ overshoot
○ ruído
○ timeout
○ ausência de progresso
○ falhas de sensor
○ falhas de dispenser
○ cenários configuráveis
○ clock de simulação
○ testes automatizados
```

---

## 51. Fluxo completo desejado

A estratégia final pode ser visualizada assim:

```text
                SIMULAÇÃO
                    │
          ┌─────────┴─────────┐
          ▼                   ▼
     Dispenser            WeightSensor
          │                   │
          │                   │
          └───────┬───────────┘
                  ▼
             comportamento
                físico
                  │
                  ▼
           DosingController
                  │
          ┌───────┼───────┐
          ▼       ▼       ▼
        DOSING  COMPLETED ERROR
                  │
                  ▼
                  UI
```

---

## 52. Princípio final

A estratégia de simulação deste projeto pode ser resumida em cinco princípios:

1. **Simular antes de integrar o hardware.**
2. **Começar simples e aumentar a fidelidade apenas quando necessário.**
3. **Permitir cenários determinísticos e reproduzíveis.**
4. **Simular falhas que seriam difíceis ou perigosas de provocar fisicamente.**
5. **Manter a simulação atrás das abstrações de hardware.**

O objetivo não é criar um modelo físico perfeito.

O objetivo é criar um ambiente controlável no qual o comportamento do sistema possa ser validado antes de depender do ESP32-S3, HX711, célula de carga e SG90.

A relação desejada é:

```text
       DESENVOLVIMENTO
              │
              ▼
        SIMULADOR
              │
       ┌──────┴──────┐
       ▼             ▼
  comportamento    falhas
       │             │
       └──────┬──────┘
              ▼
       regras validadas
              │
              ▼
        HARDWARE REAL
              │
       ┌──────┴──────┐
       ▼             ▼
     HX711          SG90
       │             │
       └──────┬──────┘
              ▼
      comportamento físico
              │
              ▼
       ajustes finais
```

O simulador deve, portanto, ser visto como uma ferramenta permanente de engenharia e validação, e não apenas como uma etapa temporária antes do hardware.