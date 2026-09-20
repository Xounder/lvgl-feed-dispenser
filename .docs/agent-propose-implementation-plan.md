# Agent Propose Implementation Plan — Proposta inicial e o que foi seguido

## Resumo

Documento histórico (registro de uma proposta do agente) contendo:

- a **proposta inicial** de como implementar o projeto, no formato "Agent First Propose Context": Fase 1 com tudo no computador (LVGL no PC, sem ESP32/tela física), o diagrama `INTERFACE LVGL → LÓGICA DO SISTEMA → HARDWARE REAL | SIMULAÇÃO`, a criação da interface completa (telas), a simulação do HX711 e do servo, a máquina de estados, a arquitetura recomendada, a validação em 3 níveis e as etapas seguintes;
- a pergunta do usuário sobre ter seguido (ou não) aquelas abordagens;
- a **explicação do agente após a implementação atual em `src`**: comparação direta entre a proposta e o que foi feito, o que foi seguido, o que "não foi" seguido (C em vez de C++, máquina de estados parcial, sem camada intermediária no controller e sem Wokwi por enquanto) e por quê.

Essa explicação também registra que a estrutura de pastas proposta tinha versões alternativas (`interfaces → simulado/real`) e conclui que **"Esse é provavelmente o próximo refinamento arquitetural relevante quando começarmos a preparar a implementação real do ESP32-S3"**.

## Índice

| Parte | Conteúdo |
| ----- | -------- |
| [01-contexto-e-fase1.md](agent-propose-implementation-plan/01-contexto-e-fase1.md) | Contexto da proposta, diagrama `INTERFACE LVGL → LÓGICA DO SISTEMA → HARDWARE REAL/SIMULAÇÃO` e Fase 1 (tudo no computador, janela que representa a futura tela) |
| [02-fases-2-e-3.md](agent-propose-implementation-plan/02-fases-2-e-3.md) | Fase 2 (criar a interface completa: telas Início, Configurar massa, Dosando, Concluído, Erro) e Fase 3 (simular o HX711: `IWeightSensor`, versão real e versão simulada) |
| [03-fases-4-e-5.md](agent-propose-implementation-plan/03-fases-4-e-5.md) | Fase 4 (simular o servo: `IDispenser`, versão simulada), "a simulação pode ser ainda melhor" e Fase 5 (simular erros, exemplo do HX711 desconectado) |
| [04-fase-6-e-arquitetura.md](agent-propose-implementation-plan/04-fase-6-e-arquitetura.md) | Fase 6 (máquina de estados, coração do projeto, lógica funcionando sem LVGL), arquitetura recomendada e o fluxo "Hoje" (PC) / "Depois" (ESP32-S3) |
| [05-validacao-wokwi-e-etapas.md](agent-propose-implementation-plan/05-validacao-wokwi-e-etapas.md) | Validação em 3 níveis (interface, lógica, simulação completa), Wokwi como complemento e as etapas "o que eu faria no seu lugar, começando agora" |
| [06-explicacao-comparacao-e-abstracoes.md](agent-propose-implementation-plan/06-explicacao-comparacao-e-abstracoes.md) | Pergunta do usuário e explicação: comparação direta (proposta × feito), C em vez de C++ com `struct` + function pointers, "simular primeiro" e o HX711 simulado |
| [07-explicacao-estados-ui-e-diferencas.md](agent-propose-implementation-plan/07-explicacao-estados-ui-e-diferencas.md) | Explicação: máquina de estados parcial, arquitetura de telas, separação UI → domínio, o que **NÃO** foi feito exatamente, não começar pelo hardware, Wokwi não seguido e a documentação criada |
| [08-explicacao-porque-e-resumo.md](agent-propose-implementation-plan/08-explicacao-porque-e-resumo.md) | Explicação: por que não seguimos 100% (evolução incremental), estado do projeto hoje, resumo ("sim, seguimos a abordagem original") e o futuro refinamento |

## Documentos relacionados

- [README.md](README.md) — índice geral da documentação
- [02-architecture.md](02-architecture.md) — arquitetura e responsabilidades
- [04-domain-and-state-machine.md](04-domain-and-state-machine.md) — domínio, estados e regras da dosagem
- [05-hardware-abstraction.md](05-hardware-abstraction.md) — abstrações de hardware
- [06-simulation-strategy.md](06-simulation-strategy.md) — estratégia de simulação
- [07-ui-and-navigation.md](07-ui-and-navigation.md) — UI e navegação
- [11-migration-pc-to-esp32.md](11-migration-pc-to-esp32.md) — migração do PC para o ESP32-S3
- [13-agent-guide.md](13-agent-guide.md) — guia para agentes que modificarem o projeto
- [implementation.md](implementation.md) — plano inicial do agente do que seria feito