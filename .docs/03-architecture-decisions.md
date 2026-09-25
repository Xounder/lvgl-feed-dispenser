# Architecture Decisions — Decisões Arquiteturais

## Resumo

Este documento registra as principais decisões arquiteturais tomadas durante o desenvolvimento do projeto e, principalmente, **por que essas decisões foram tomadas**.

O objetivo não é afirmar que essas escolhas são universalmente as melhores, mas preservar o contexto necessário para que futuras alterações sejam feitas conscientemente.

Um agente ou desenvolvedor que trabalhar no projeto deve consultar este documento antes de substituir tecnologias, remover abstrações ou alterar a estratégia de desenvolvimento.

## Índice

| Parte | Conteúdo |
| ----- | -------- |
| [01-linguagem-e-build.md](03-architecture-decisions/01-linguagem-e-build.md) | Decisão de usar C++ (com C no LVGL/SDL2 e no domínio atual), CMake/vcpkg no PC e Arduino IDE + core ESP32 no alvo |
| [02-interface-grafica.md](03-architecture-decisions/02-interface-grafica.md) | Decisões de UI: LVGL e SDL2 no simulador |
| [03-estrategia-de-desenvolvimento.md](03-architecture-decisions/03-estrategia-de-desenvolvimento.md) | Simulador antes do ESP32, simulador permanente, evolução incremental |
| [04-separacao-e-abstracoes.md](03-architecture-decisions/04-separacao-e-abstracoes.md) | Separação domínio/hardware, interfaces, abstrações justificadas |
| [05-ui-estados-e-controller.md](03-architecture-decisions/05-ui-estados-e-controller.md) | UI separada das regras, DosingController, estados explícitos, configuração separada |
| [06-contexto-e-resumo.md](03-architecture-decisions/06-contexto-e-resumo.md) | Preservação do contexto das escolhas, resumo das decisões e princípio final |

## Documentos relacionados

- [02-architecture.md](02-architecture.md) — arquitetura e responsabilidades
- [04-domain-and-state-machine.md](04-domain-and-state-machine.md) — domínio e máquina de estados
- [05-hardware-abstraction.md](05-hardware-abstraction.md) — abstrações de hardware
- [06-simulation-strategy.md](06-simulation-strategy.md) — estratégia de simulação
- [07-ui-and-navigation.md](07-ui-and-navigation.md) — UI e navegação
- [11-migration-pc-to-esp32.md](11-migration-pc-to-esp32.md) — migração do PC para o ESP32-S3