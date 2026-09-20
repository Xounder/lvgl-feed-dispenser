# Fundamentos e níveis de simulação

## 1. Objetivo

O simulador desktop é uma parte importante do desenvolvimento do projeto.

Ele existe para permitir que o comportamento do dosador seja desenvolvido, observado e testado antes da disponibilidade e integração do hardware físico.

A simulação deve representar as partes do sistema que influenciam o comportamento da aplicação, principalmente:

* peso;
* liberação de ração;
* evolução da dosagem;
* tempo;
* falhas;
* condições anormais;
* conclusão;
* cancelamento.

A intenção não é criar uma simulação física perfeita.

O objetivo é criar uma representação suficientemente realista e controlável para responder a perguntas como:

```text
O controller para quando atingir o peso?

O sistema reage corretamente a uma dosagem lenta?

O que acontece se o peso não aumentar?

O que acontece se o sensor falhar?

O que acontece se houver overshoot?

O cancelamento realmente interrompe a dosagem?

O sistema consegue detectar uma situação que nunca termina?
```

---

## 2. Por que simular antes do hardware

O hardware físico introduz várias variáveis:

```text
HX711
Load Cell
SG90
mecanismo mecânico
ração
reservatório
vibração
alimentação elétrica
calibração
ruído
```

Se a lógica da aplicação fosse desenvolvida diretamente sobre esses componentes, seria difícil determinar se um problema está:

```text
na lógica
```

ou:

```text
no hardware
```

ou:

```text
na calibração
```

ou:

```text
na mecânica
```

O simulador reduz essa complexidade.

---

## 3. Simulação como ambiente de desenvolvimento

O simulador não deve ser tratado como:

```text
"protótipo descartável"
```

Ele é um ambiente de desenvolvimento.

A relação desejada é:

```text
              SISTEMA
                 │
        ┌────────┴────────┐
        ▼                 ▼
     SIMULADOR          HARDWARE
        │                 │
        ▼                 ▼
 comportamento       comportamento
    validado            físico
```

O simulador permite validar a lógica antes da integração física.

---

## 4. O que deve ser simulado

As principais partes são:

```text
1. Peso
2. Dispenser
3. Fluxo de ração
4. Tempo
5. Ruído
6. Overshoot
7. Falta de progresso
8. Falhas do sensor
9. Falhas do dispenser
10. Cancelamento
11. Timeout
```

Nem todas precisam existir imediatamente.

A evolução deve acontecer de forma incremental.

---

## 5. Níveis de simulação

A simulação pode ser dividida em níveis.

### Nível 1 — Fluxo básico

Já implementado.

```text
dispenser ativo
       ↓
peso + 2 g
       ↓
controller verifica peso
       ↓
atingiu objetivo?
       ↓
sim → completa
```

### Nível 2 — Fluxo temporal

Considerar:

```text
tempo
taxa de alimentação
```

### Nível 3 — Comportamento físico simplificado

Adicionar:

```text
variação
atraso
overshoot
ruído
```

### Nível 4 — Falhas

Adicionar:

```text
sensor travado
sensor inválido
dispenser travado
fluxo insuficiente
ausência de progresso
```

### Nível 5 — Cenários configuráveis

Permitir selecionar diferentes condições de teste.

Cada nível é implementado sobre os componentes simulados `WeightSensor`/`Dispenser` definidos em [05-hardware-abstraction.md](../05-hardware-abstraction.md). Os estados de dosagem reproduzidos (`DOSING`, `COMPLETED`, `ERROR`, `IDLE`) pertencem ao domínio, descrito em [04-domain-and-state-machine.md](../04-domain-and-state-machine.md).