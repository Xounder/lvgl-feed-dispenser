# Implementation — Plano inicial e o que foi seguido

## Resumo

Documento histórico com o **plano inicial do agente** de "o que seria feito" para o projeto e, posteriormente, a **explicação do agente após a implementação atual em `src`**.

O plano sugere a organização de `pc-vscode/` com `config/`, `src/main.c`, `hal/`, `app/`, `domain/` (`dosing_controller.*`, `dosing_mode.h`, `system_state.h`), `hardware/` (`weight_sensor.h`, `dispenser.h`, `simulated/`), `ui/` (`ui.c`, `screens/home...completed`, `components/header`, `button`), `lv_conf.h` e `CMakeLists.txt`, com o princípio de que **`main.c` fica extremamente pequeno**, evoluindo **por etapas** ("não criaria tudo agora. Vamos evoluir por etapas.").

A explicação compara o plano com o que foi feito em `src`, registrando que a arquitetura e o fluxo principais foram seguidos, mas que a camada `app/` explícita **não foi criada** (seria uma abstração prematura naquele momento), que os componentes genéricos de UI não foram antecipados e que a simulação entrou antes do previsto no plano.

## Índice

| Parte | Conteúdo |
| ----- | -------- |
| [01-organizacao-e-main-c.md](implementation/01-organizacao-e-main-c.md) | Organização proposta de `pc-vscode/` (`config/`, `src/`, `hal/`, `app/`, `domain/`, `hardware/`, `ui/`, `lv_conf.h`, `CMakeLists.txt`) e o `main.c` extremamente pequeno |
| [02-camada-ui-e-screen-manager.md](implementation/02-camada-ui-e-screen-manager.md) | Camada `ui` (fluxo Home → Seleção de modo → Configuração → Dosagem → Concluído), `ScreenManager` e a regra de não misturar tela com lógica da dosagem |
| [03-cinco-telas-e-simulacao.md](implementation/03-cinco-telas-e-simulacao.md) | As 5 telas iniciais (Home, Seleção de modo, Configuração, Dosagem, Concluído), a entrada da simulação depois e a ordem de implementação |
| [04-explicacao-comparacao-mainc-ui-e-telas.md](implementation/04-explicacao-comparacao-mainc-ui-e-telas.md) | Explicação: comparação do plano com o que foi feito, `main.c` no espírito do plano, camada `ui/` e `ScreenManager` implementados, as cinco telas |
| [05-explicacao-app-e-screen-manager.md](implementation/05-explicacao-app-e-screen-manager.md) | Explicação: a camada `app/` **não criada** (e por que não foi um erro) e a diferença entre `ScreenManager` e uma futura camada `App` |
| [06-explicacao-simulacao-estados-e-docs.md](implementation/06-explicacao-simulacao-estados-e-docs.md) | Explicação: simulação entrou antes do previsto, sensor/dispenser simulados, dosagem simulada, `DosingConfig`, máquina de estados parcial e a separação Atual/Planejado nos documentos |
| [07-explicacao-evolucao-e-avaliacao.md](implementation/07-explicacao-evolucao-e-avaliacao.md) | Explicação: a evolução real (diagramas atual e futuro) e a avaliação final do plano |

## Documentos relacionados

- [README.md](README.md) — índice geral da documentação
- [agent-propose-implementation-plan.md](agent-propose-implementation-plan.md) — proposta inicial do agente e o que foi seguido
- [02-architecture.md](02-architecture.md) — arquitetura e responsabilidades
- [04-domain-and-state-machine.md](04-domain-and-state-machine.md) — domínio, estados e regras da dosagem
- [05-hardware-abstraction.md](05-hardware-abstraction.md) — abstrações de hardware
- [06-simulation-strategy.md](06-simulation-strategy.md) — estratégia de simulação
- [07-ui-and-navigation.md](07-ui-and-navigation.md) — UI e navegação
- [13-agent-guide.md](13-agent-guide.md) — guia para agentes que modificarem o projeto