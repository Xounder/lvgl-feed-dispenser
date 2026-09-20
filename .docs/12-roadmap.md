# Roadmap do Projeto

> **Documento canônico** de **roadmap, estado atual e próximos passos** do dosador de ração.
> Este arquivo é o índice; o conteúdo completo está nas partes listadas abaixo.
> Outras partes da documentação devem linkar para este arquivo ao tratar de direção do projeto, evolução planejada, fases do roadmap, estado atual do sistema, critérios para avançar e o que vem a seguir.

---

## Resumo

Este documento apresenta a evolução planejada do projeto desde o início do simulador desktop até a integração com o hardware físico baseado em ESP32-S3.

O roadmap existe para manter uma visão clara de:

* o que já foi concluído;
* o que está sendo desenvolvido;
* qual é o próximo passo;
* quais funcionalidades ainda são planejadas;
* quais etapas dependem do hardware físico.

O roadmap não deve ser interpretado como um cronograma rígido: a ordem pode mudar conforme a disponibilidade dos componentes, descobertas durante os testes, problemas de arquitetura, limitações do hardware e necessidades do projeto.

---

## Estado atual

Atualmente o projeto já possui um simulador funcional no PC.

O fluxo principal está implementado:

```text
Home
  ↓
Seleção de modo
  ↓
Configuração
  ↓
Dosagem
  ↓
Conclusão
```

A dosagem possui um peso simulado e um dispenser simulado.

O ciclo completo já pode ser executado sem o hardware físico.

---

## Próxima fase

O próximo grande ciclo é a **robustez do simulador**, antes de iniciar a migração física:

```text
1. melhorar o comportamento do domínio
2. tratar estados de erro
3. adicionar timeout
4. simular sensor sem progresso
5. tratar excesso de peso
6. fortalecer os testes
```

O detalhamento técnico é tratado nos documentos canônicos de domínio, simulação e testes:

- [`04-domain-and-state-machine.md`](04-domain-and-state-machine.md) — estados, `ERROR`, timeout e validação.
- [`06-simulation-strategy.md`](06-simulation-strategy.md) — sensor parado, overshoot e falhas simuladas.
- [`10-testing-strategy.md`](10-testing-strategy.md) — testes automatizados e casos de erro.

---

## Índice

| Parte | Conteúdo |
| ----- | -------- |
| [`12-roadmap/01-visao-geral-e-estado-atual.md`](12-roadmap/01-visao-geral-e-estado-atual.md) | Objetivo do roadmap (não é cronograma rígido), visão geral da evolução (`Ideia → Simulador PC → … → Produto funcional`) e estado atual do projeto. |
| [`12-roadmap/02-o-que-ja-foi-concluido.md`](12-roadmap/02-o-que-ja-foi-concluido.md) | Ambiente de desenvolvimento, repositório, simulador desktop, interface inicial, seleção de modo, configurações (quantidade fixa e porções), controller, abstração de hardware, sensor/dispenser simulados, cancelamento, reset, conclusão, testes manuais e documentação. |
| [`12-roadmap/03-proximo-ciclo-robustez.md`](12-roadmap/03-proximo-ciclo-robustez.md) | Próximo passo imediato e itens de robustez: estado `ERROR`, timeout, sensor sem progresso, overshoot, validação de configuração, testes automatizados, simulador mais realista e organização futura das implementações. |
| [`12-roadmap/04-migracao-esp32-e-dosagem-fisica.md`](12-roadmap/04-migracao-esp32-e-dosagem-fisica.md) | Preparação para o ESP32-S3, display + touch, port da UI, HX711 + load cell, SG90 + mecanismo, integração mecânica, primeiros testes físicos, ajuste da dosagem, calibração, repetibilidade, falhas no hardware e teste de energia. |
| [`12-roadmap/05-fases-dependencias-e-criterios.md`](12-roadmap/05-fases-dependencias-e-criterios.md) | Roadmap por fases (Fases 1–12), roadmap visual com legendas (✓/→/○), dependências entre fases, o que pode acontecer em paralelo e critério para avançar. |
| [`12-roadmap/06-definicoes-abertas-e-visao-final.md`](12-roadmap/06-definicoes-abertas-e-visao-final.md) | O que ainda não está definido, o roadmap não é contrato, regra para atualização do roadmap, visão de longo prazo, estado final esperado e princípio final. |

---

## Fases em resumo

Fases concluídas:

```text
Fase 1 — Fundação          ✓
Fase 2 — UI                ✓
Fase 3 — Domínio           ✓
Fase 4 — Hardware simulado ✓
```

Próximo ciclo:

```text
Fase 5 — Robustez do simulador → próximo ciclo de desenvolvimento
```

Fases planejadas:

```text
Fase 6 — ESP32-S3
Fase 7 — Display e Touch
Fase 8 — Pesagem
Fase 9 — Dispenser físico
Fase 10 — Dosagem física
Fase 11 — Robustez física
Fase 12 — Produto final
```

---

## Componentes-chave

- `DosingConfig` / `DosingController` — domínio de dosagem já implementado em forma inicial.
- `WeightSensor` / `Dispenser` — abstrações de hardware com implementações simuladas.
- Estados atuais: `IDLE` → `DOSING` → `COMPLETED`; evolução planejada inclui `SELECT_MODE`, `CONFIGURING` e `ERROR`.

A abstração de hardware usada pelo domínio é tratada em [`05-hardware-abstraction.md`](05-hardware-abstraction.md), e a UI em [`07-ui-and-navigation.md`](07-ui-and-navigation.md).

---

## Documentos relacionados

- [`00-project-story.md`](00-project-story.md) — história e motivação do projeto.
- [`02-architecture.md`](02-architecture.md) — arquitetura e responsabilidades das camadas.
- [`03-architecture-decisions.md`](03-architecture-decisions.md) — decisões arquiteturais.
- [`04-domain-and-state-machine.md`](04-domain-and-state-machine.md) — domínio, estados e regras da dosagem.
- [`05-hardware-abstraction.md`](05-hardware-abstraction.md) — abstrações de hardware.
- [`06-simulation-strategy.md`](06-simulation-strategy.md) — estratégia de simulação.
- [`07-ui-and-navigation.md`](07-ui-and-navigation.md) — UI e navegação.
- [`08-target-hardware.md`](08-target-hardware.md) — hardware físico alvo.
- [`09-pc-development-environment.md`](09-pc-development-environment.md) — ambiente de desenvolvimento no PC.
- [`10-testing-strategy.md`](10-testing-strategy.md) — estratégia de testes.
- [`11-migration-pc-to-esp32.md`](11-migration-pc-to-esp32.md) — migração do PC para o ESP32-S3.

---

## Princípio final

> **Primeiro provar o comportamento. Depois integrar o hardware. Depois calibrar a física.**

O projeto já possui um simulador funcional com **UI + Domain + Hardware abstractions + Simulated hardware**. O próximo grande ciclo é aumentar a robustez dessa base; depois disso, a migração deve ocorrer gradualmente, levando para o ESP32 uma arquitetura já compreendida e testada no simulador.