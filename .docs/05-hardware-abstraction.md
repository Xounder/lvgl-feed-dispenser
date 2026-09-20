# Hardware Abstraction — Abstração de Hardware

> **Documento canônico** de **abstração de hardware** do dosador de ração.
> Este arquivo é o índice; o conteúdo completo está nas partes listadas abaixo.
> Outras partes da documentação devem linkar para este arquivo ao tratar de `WeightSensor`, `Dispenser`, implementações simuladas/reais (`HX711`/`SG90`), encapsulamento de detalhes físicos e migração de hardware.

---

## Resumo

Este documento descreve como o projeto representa sensores e atuadores através de abstrações de hardware.

O objetivo principal é permitir que a lógica de dosagem seja desenvolvida e testada sem depender diretamente do hardware físico.

Atualmente, o projeto possui duas abstrações principais:

```text
WeightSensor
Dispenser
```

Cada uma representa uma capacidade que o domínio precisa utilizar.

As implementações atuais são simuladas:

```text
SimulatedWeightSensor
SimulatedDispenser
```

No futuro, deverão existir implementações reais para:

```text
HX711 + Load Cell
SG90 / atuador
```

A ideia central:

```text
DosingController
       │
       ├───────────────┐
       ▼               ▼
 WeightSensor       Dispenser
       │               │
   ┌───┴───┐       ┌───┴───┐
   ▼       ▼       ▼       ▼
Simulado  Real  Simulado  Real
```

---

## Índice

| Parte | Conteúdo |
| ----- | -------- |
| [`05-hardware-abstraction/01-fundamentos-e-estrutura.md`](05-hardware-abstraction/01-fundamentos-e-estrutura.md) | Objetivo, princípio da abstração, estrutura conceitual e localização no projeto. |
| [`05-hardware-abstraction/02-weight-sensor.md`](05-hardware-abstraction/02-weight-sensor.md) | Interface, responsabilidades, implementação simulada e futuro sensor HX711 (calibração, tara, filtragem). |
| [`05-hardware-abstraction/03-dispenser.md`](05-hardware-abstraction/03-dispenser.md) | Interface, responsabilidades, implementação simulada e futuro dispenser SG90. |
| [`05-hardware-abstraction/04-relacao-com-dominio-e-falhas.md`](05-hardware-abstraction/04-relacao-com-dominio-e-falhas.md) | Relação com o domínio, ciclos de exemplo, erros de sensor/dispenser, simulação de falhas e tabela simulada vs. real. |
| [`05-hardware-abstraction/05-evolucao-e-migracao.md`](05-hardware-abstraction/05-evolucao-e-migracao.md) | Evolução das interfaces, composição, seleção, migração, testes, segurança, inicialização e princípios finais. |

---

## Componentes-chave

- `WeightSensor` — interface com `read_grams()`, `add_grams()` e `reset()`; implementação atual `simulated_weight_sensor`.
- `Dispenser` — interface com `start()`, `stop()` e `is_active()`; implementação atual `simulated_dispenser`.
- Localização: `src/hardware/`.

As regras de domínio que consomem essas abstrações são tratadas em [`04-domain-and-state-machine.md`](04-domain-and-state-machine.md).