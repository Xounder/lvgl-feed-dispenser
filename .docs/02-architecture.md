# Architecture — Arquitetura Geral

## Resumo

Este documento descreve a arquitetura do projeto, suas camadas, responsabilidades, dependências e fluxo de comunicação.

A arquitetura foi construída para permitir que o mesmo núcleo de lógica seja utilizado tanto no **simulador desktop** quanto, posteriormente, no **ESP32-S3**.

O princípio central — *a lógica de negócio deve depender de abstrações* — e a visão das camadas estão na parte [01-principio-e-camadas.md](02-architecture/01-principio-e-camadas.md).

## Índice

| Parte | Conteúdo |
| ----- | -------- |
| [01-principio-e-camadas.md](02-architecture/01-principio-e-camadas.md) | Objetivo, estrutura conceitual, princípio arquitetural central e as camadas |
| [02-responsabilidades-ui-e-dominio.md](02-architecture/02-responsabilidades-ui-e-dominio.md) | Responsabilidades da UI, do domínio, do `DosingConfig` e do `DosingController`, e o que o domínio deve/não deve conhecer |
| [03-abstracoes-de-hardware-e-dependencias.md](02-architecture/03-abstracoes-de-hardware-e-dependencias.md) | Abstrações de hardware, direção de dependências entre camadas e dependências evitadas |
| [04-fluxos-telas-e-atualizacao.md](02-architecture/04-fluxos-telas-e-atualizacao.md) | Fluxo de uma dosagem, fluxo entre UI e controller, fluxo de atualização, gerenciamento de telas e relação estado/tela |
| [05-estrutura-hal-e-migracao.md](02-architecture/05-estrutura-hal-e-migracao.md) | Estrutura `src/`, `main.c`/HAL, fronteira PC→ESP32, diretriz para novos códigos e regra contra acoplamento |
| [06-qualidade-e-evolucao.md](02-architecture/06-qualidade-e-evolucao.md) | Importância da separação, testabilidade, evolução das abstrações, estado atual versus arquitetura final e resumo |

## Documentos relacionados

- [03-architecture-decisions.md](03-architecture-decisions.md) — decisões arquiteturais e seus motivos
- [04-domain-and-state-machine.md](04-domain-and-state-machine.md) — domínio, estados e regras da dosagem
- [05-hardware-abstraction.md](05-hardware-abstraction.md) — abstrações de hardware
- [07-ui-and-navigation.md](07-ui-and-navigation.md) — UI e navegação
- [11-migration-pc-to-esp32.md](11-migration-pc-to-esp32.md) — migração do PC para o ESP32-S3