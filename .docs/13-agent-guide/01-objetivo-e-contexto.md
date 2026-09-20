## 01. Objetivo e contexto

[voltar ao índice](../13-agent-guide.md)

---

## 1. Objetivo deste documento

Este documento serve como guia para qualquer agente de IA que venha a analisar, modificar, testar ou evoluir este projeto.

O objetivo é evitar que um agente precise reconstruir todo o contexto do projeto a cada tarefa.

Antes de modificar o código, o agente deve entender:

* o objetivo do projeto;
* a arquitetura atual;
* o que já está implementado;
* o que ainda é apenas planejado;
* quais decisões arquiteturais já foram tomadas;
* quais partes são específicas do PC;
* quais partes deverão migrar para o ESP32-S3;
* quais mudanças podem afetar o hardware futuro.

A regra principal é:

> **Não modificar o projeto apenas para fazê-lo funcionar localmente. Modificar o projeto preservando sua evolução planejada para o hardware físico.**

---

## 2. Leia a documentação antes de modificar

A documentação em `.docs/` faz parte do contexto técnico do projeto.

Antes de realizar uma mudança relevante, consulte principalmente:

* [00-project-story.md](../00-project-story.md)
* [01-project-overview.md](../01-project-overview.md)
* [02-architecture.md](../02-architecture.md)
* [03-architecture-decisions.md](../03-architecture-decisions.md)
* [04-domain-and-state-machine.md](../04-domain-and-state-machine.md)
* [07-ui-and-navigation.md](../07-ui-and-navigation.md)
* [08-target-hardware.md](../08-target-hardware.md)
* [09-pc-development-environment.md](../09-pc-development-environment.md)
* [10-testing-strategy.md](../10-testing-strategy.md)
* [11-migration-pc-to-esp32.md](../11-migration-pc-to-esp32.md)
* [12-roadmap.md](../12-roadmap.md)
* [13-agent-guide.md](../13-agent-guide.md)

A ordem recomendada de leitura é:

```text
00 → 01 → 02 → 03 → 04
                  ↓
             07 → 08
                  ↓
             09 → 10 → 11 → 12
```

Não é necessário ler todos os arquivos para uma alteração trivial.

Porém, para alterações arquiteturais, de domínio ou de hardware, a documentação correspondente deve ser consultada.

---

## 3. Entenda o objetivo do projeto

O projeto é um sistema de dosagem automática de ração.

A aplicação está sendo desenvolvida inicialmente em um simulador desktop utilizando:

```text
C
LVGL
SDL2
CMake
MSVC
```

O objetivo não é criar apenas uma interface simulada.

O simulador existe para desenvolver e validar o comportamento que posteriormente deverá funcionar em um:

```text
ESP32-S3 N16R8
```

com:

```text
display 4.3"
800×480
touch capacitivo
HX711
load cell
SG90
mecanismo de dispenser
```

Portanto:

> **O simulador é parte do processo de desenvolvimento do produto final.**

Ele não deve ser tratado como um protótipo descartável.

---

## 4. Regra mais importante: distinguir estado atual de arquitetura futura

A documentação descreve tanto:

* o que já existe;
* quanto o que foi planejado.

Essas duas coisas não são necessariamente iguais.

Por exemplo, conceitualmente a arquitetura desejada é:

```text
UI
 ↓
DosingController
 ↓
Hardware Interface
 ├── WeightSensor
 └── Dispenser
      ├── Simulated
      └── Real
```

Porém, a implementação atual do controller ainda referencia diretamente:

```text
simulated_weight_sensor
simulated_dispenser
```

Isso é uma característica do estágio atual.

Não assumir que algo descrito como arquitetura futura já foi implementado.

---

## 5. Não invente funcionalidades existentes

Antes de afirmar que uma funcionalidade existe, procure no código.

Exemplo:

A documentação descreve o estado futuro:

```text
IDLE
SELECT_MODE
CONFIGURING
DOSING
COMPLETED
ERROR
```

Mas a enumeração atualmente implementada é:

```c
typedef enum {
    DOSING_STATE_IDLE,
    DOSING_STATE_DOSING,
    DOSING_STATE_COMPLETED
} DosingState;
```

Portanto:

```text
ERROR
```

é planejamento, não implementação atual.

O mesmo vale para:

* timeout;
* sensor sem progresso;
* overshoot controlado;
* filtragem real;
* tratamento de falha física;
* testes automatizados;
* hardware real.