# Guia para Agentes

## Resumo

Este documento é o guia canônico de regras e orientações para qualquer agente de IA que venha a analisar, modificar, testar ou evoluir este projeto.

O objetivo é evitar que um agente precise reconstruir todo o contexto do projeto a cada tarefa.

A regra principal é:

> **Não modificar o projeto apenas para fazê-lo funcionar localmente. Modificar o projeto preservando sua evolução planejada para o hardware físico.**

Este guia cobre:

* como o agente deve continuar o projeto;
* a distinção entre **O QUE EXISTE HOJE**, **O QUE É ARQUITETURA DESEJADA**, **O QUE É PLANEJADO** e **O QUE AINDA NÃO FOI DECIDIDO**;
* as regras que devem ser consideradas antes de alterar arquitetura, domínio, UI, simulação ou hardware;
* o contexto do que está implementado (estrutura de pastas, CMake, LVGL/SDL2, `DosingController`, sensores/atuadores simulados, telas, fluxo, limitações);
* `MAIN_SOURCES` no `src_pc/CMakeLists.txt` para novos `.c`;
* a regra de que a simulação não deve ser removida quando o hardware chegar;
* a distinção do timer ~300 ms de UI em relação à temporização física;
* a observação de que a fonte LVGL atual não possui acentos.

## Índice

| Parte | Conteúdo |
| ----- | -------- |
| [01-objetivo-e-contexto.md](13-agent-guide/01-objetivo-e-contexto.md) | Objetivo do guia, leitura da documentação, objetivo do projeto, regra do estado atual versus arquitetura futura e não inventar funcionalidades |
| [02-arquitetura-e-camadas.md](13-agent-guide/02-arquitetura-e-camadas.md) | Arquitetura atual, responsabilidade de cada camada, regra de dependência, UI sem regra de negócio, não colocar LVGL/SDL2/Windows no domínio, hardware abstrato e simulação permanente |
| [03-dominio-e-regras-de-dosagem.md](13-agent-guide/03-dominio-e-regras-de-dosagem.md) | Estado de negócio ≠ tela, máquina de estados, `DosingConfig`, regras atuais de dosagem, interrupção segura, comportamento físico, overshoot, erros planejados, validação e memória |
| [04-codigo-e-dependencias.md](13-agent-guide/04-codigo-e-dependencias.md) | Não refatorar sem necessidade, abstrações prematuras, CMake/`MAIN_SOURCES`, dependências externas, SDL2, LVGL, ThorVG, `main.c`, HAL, telas, navegação, textos sem acentos e timer da tela de dosagem |
| [05-testes-e-validacao.md](13-agent-guide/05-testes-e-validacao.md) | Testar antes de concluir, regra de regressão, testes reproduzíveis, não depender do hardware, hardware como fonte de informação e regras de migração entre PC e ESP32 |
| [06-hardware.md](13-agent-guide/06-hardware.md) | Hardware futuro, pinagem, segurança do atuador e alimentação |
| [07-refatoracao-e-mudancas.md](13-agent-guide/07-refatoracao-e-mudancas.md) | Como adicionar funcionalidades, exemplos de sensor/tela de erro/timeout, quando refatorar e quando não refatorar, não trocar C, LVGL ou build |
| [08-erros-e-checklists.md](13-agent-guide/08-erros-e-checklists.md) | Como lidar com bugs, comportamento já validado e checklists antes/depois de modificar (domínio, hardware, UI e CMake) |
| [09-informacao-desconhecida-e-dependencias.md](13-agent-guide/09-informacao-desconhecida-e-dependencias.md) | Informação desconhecida, hardware indisponível, novas dependências, novas abstrações e comentários |
| [10-documentacao-e-principios.md](13-agent-guide/10-documentacao-e-principios.md) | Regra para documentação, hipóteses, prioridade das informações, decisões antigas, simulador como contrato, filosofias e princípios |
| [11-exemplos-e-estado-final.md](13-agent-guide/11-exemplos-e-estado-final.md) | Exemplos completos de raciocínio, critério de qualidade, ordem de grandes alterações e estado final esperado |

## Documentos relacionados

- [00-project-story.md](00-project-story.md) — contexto e histórico do projeto
- [01-project-overview.md](01-project-overview.md) — visão geral
- [02-architecture.md](02-architecture.md) — arquitetura e camadas
- [03-architecture-decisions.md](03-architecture-decisions.md) — decisões arquiteturais
- [04-domain-and-state-machine.md](04-domain-and-state-machine.md) — domínio e máquina de estados
- [07-ui-and-navigation.md](07-ui-and-navigation.md) — UI e navegação
- [08-target-hardware.md](08-target-hardware.md) — hardware alvo
- [09-pc-development-environment.md](09-pc-development-environment.md) — ambiente de desenvolvimento no PC
- [10-testing-strategy.md](10-testing-strategy.md) — estratégia de testes
- [11-migration-pc-to-esp32.md](11-migration-pc-to-esp32.md) — migração PC → ESP32
- [12-roadmap.md](12-roadmap.md) — etapas planejadas