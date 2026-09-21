# Plano de Migração: PC → ESP32-S3

> **Documento canônico** de **migração do simulador PC para o ESP32-S3** do dosador de ração.
> Este arquivo é o índice; o conteúdo completo está nas partes listadas abaixo.
> Outras partes da documentação devem linkar para este arquivo ao tratar do processo de migração: alimentação como ponto crítico no físico, GPIOs inexistentes no simulador, proposta `hardware/board_config`, migração incremental e valores de temporização do simulador que não são definitivos.

---

## Resumo

Este documento define como o projeto deve evoluir do simulador desktop para o hardware físico baseado em ESP32-S3.

A migração não deve ser tratada como uma reescrita completa: partes específicas da plataforma são substituídas enquanto o comportamento da aplicação permanece o mais estável possível.

```text
PC
├── Windows
├── SDL2
├── SDL HAL
├── hardware simulado
└── aplicação
        │
        ▼
ESP32-S3
├── ESP32
├── display/touch real
├── HAL/driver
├── hardware real
└── mesma aplicação
```

A regra principal:

> **Substituir implementações específicas da plataforma, não reescrever o comportamento do produto sem necessidade.**

Os detalhes específicos de hardware físico (display, touch, HX711, servo, alimentação) são tratados em [08-target-hardware.md](08-target-hardware.md); as abstrações `WeightSensor`/`Dispenser` em [05-hardware-abstraction.md](05-hardware-abstraction.md); a estratégia de simulação em [06-simulation-strategy.md](06-simulation-strategy.md).

---

## Índice

| Parte | Conteúdo |
| ----- | -------- |
| [01-objetivo-e-principio.md](11-migration-pc-to-esp32/01-objetivo-e-principio.md) | Objetivo, diagrama PC → ESP32-S3, visão geral da migração e princípio (substituir plataformas, não reescrever o comportamento) |
| [02-permanece-substitui-e-comparacao.md](11-migration-pc-to-esp32/02-permanece-substitui-e-comparacao.md) | O que permanece, o que será substituído, tabela de comparação PC × ESP32-S3 e o que não deve ser migrado |
| [03-lvgl-ui-e-interacao-fisica.md](11-migration-pc-to-esp32/03-lvgl-ui-e-interacao-fisica.md) | LVGL permanece, UI durante a migração, resolução, touch, mouse versus touch, botões físicos, switch e LED/indicador (GPIOs na camada de hardware) |
| [04-sensor-peso-e-dispenser.md](11-migration-pc-to-esp32/04-sensor-peso-e-dispenser.md) | WeightSensor, HX711, calibração, ruído, Dispenser, SG90, servos e overshoot físico (com links para os donos canônicos) |
| [05-controller-estados-e-separacao.md](11-migration-pc-to-esp32/05-controller-estados-e-separacao.md) | DosingController, exemplo da migração, estados, o que não fazer e separação desejada |
| [06-build-plataforma-memoria-e-assets.md](11-migration-pc-to-esp32/06-build-plataforma-memoria-e-assets.md) | Build system (PlatformIO + Arduino/ESP-IDF), FreeRTOS, LVGL no ESP32, buffers e memória, performance e assets |
| [07-tempos-gpios-board-config-e-incremento.md](11-migration-pc-to-esp32/07-tempos-gpios-board-config-e-incremento.md) | Tempo e timers (300 ms), tempo real, alimentação como ponto crítico, GPIOs, proposta `hardware/board_config` e migração incremental |
| [08-etapas-da-migracao.md](11-migration-pc-to-esp32/08-etapas-da-migracao.md) | Etapas 1 a 10 da migração incremental, ajuste da vazão e ajuste do ponto de parada |
| [09-estrutura-e-duas-plataformas.md](11-migration-pc-to-esp32/09-estrutura-e-duas-plataformas.md) | Migração da simulação, estrutura após a migração, PC e ESP32 como duas plataformas, fronteiras de dependência e evolução da arquitetura |
| [10-checklist-criterio-e-evolucao.md](11-migration-pc-to-esp32/10-checklist-criterio-e-evolucao.md) | Checklist de migração, critério de conclusão, simulador depois da migração, estratégia de desenvolvimento, riscos e princípio final |

---

## Documentos relacionados

- [02-architecture.md](02-architecture.md) — arquitetura e responsabilidades das camadas
- [04-domain-and-state-machine.md](04-domain-and-state-machine.md) — domínio, estados e regras da dosagem
- [05-hardware-abstraction.md](05-hardware-abstraction.md) — abstrações `WeightSensor` e `Dispenser`
- [06-simulation-strategy.md](06-simulation-strategy.md) — estratégia de simulação
- [07-ui-and-navigation.md](07-ui-and-navigation.md) — UI e navegação
- [08-target-hardware.md](08-target-hardware.md) — hardware físico alvo
- [09-pc-development-environment.md](09-pc-development-environment.md) — ambiente de desenvolvimento no PC