# Estratégia de Testes

> **Documento canônico** de **estratégia de testes** do dosador de ração.
> Este arquivo é o índice; o conteúdo completo está nas partes listadas abaixo.
> Outras partes da documentação devem linkar para este arquivo ao tratar de testes, validação do fluxo principal e de falhas, cenários (erros, timeout, sensor travado, sensor inválido, dispenser travado, fluxo insuficiente, ausência de progresso, overshoot, tolerância) e critérios de sucesso.

---

## Resumo

Este documento define como o projeto deve ser testado durante sua evolução no simulador PC e, posteriormente, no ESP32-S3.

O objetivo não é apenas verificar se a interface "abre e funciona".

Os testes devem validar o comportamento completo do sistema:

```text
Entrada do usuário
       ↓
Configuração
       ↓
Início da dosagem
       ↓
Leitura do peso
       ↓
Controle do dispenser
       ↓
Atingimento da meta
       ↓
Parada
       ↓
Conclusão
```

Também devem ser testadas situações anormais:

```text
interrupção / emergência
sensor sem progresso
peso acima da meta
timeout
falha de hardware
configuração inválida
```

A estratégia deve evoluir junto com o projeto.

---

## Princípio central

> **Toda operação de dosagem deve possuir um caminho normal para conclusão e um caminho seguro para interrupção.**

O objetivo final da estratégia de testes é garantir que a evolução do projeto não dependa apenas de demonstrações manuais bem-sucedidas, mas que o comportamento normal, os limites e as falhas previsíveis sejam deliberadamente exercitados antes e depois da migração para o hardware físico.

---

## Índice

| Parte | Conteúdo |
| ----- | -------- |
| [`10-testing-strategy/01-fundamentos-e-filosofia.md`](10-testing-strategy/01-fundamentos-e-filosofia.md) | Objetivo, filosofia de testes, o que deve ser testado, testes de build, inicialização, tela Home, navegação e retorno. |
| [`10-testing-strategy/02-configuracao-e-dosagem-normal.md`](10-testing-strategy/02-configuracao-e-dosagem-normal.md) | Modos de configuração (Massa e Valor R$), limites da quantidade, dosagem normal, critério de sucesso, peso crescente (etapas rápida/fina), progress bar, conclusão, nova dosagem e reset/tara. |
| [`10-testing-strategy/03-cancelamento-timeout-e-falta-de-progresso.md`](10-testing-strategy/03-cancelamento-timeout-e-falta-de-progresso.md) | Interrupção (Parar/Emergência), critério de segurança da interrupção, interrupção repetida, sensor parado, problema do sensor parado, estratégia de falta de progresso, timeout e timeout global versus falta de progresso. |
| [`10-testing-strategy/04-overshoot-e-falhas-de-sensor-e-dispenser.md`](10-testing-strategy/04-overshoot-e-falhas-de-sensor-e-dispenser.md) | Excesso de peso, overshoot, critério futuro para excesso, sensor com leitura inválida, peso negativo, falha do dispenser, dispenser ativo após conclusão/cancelamento e interrupção durante dosagem. |
| [`10-testing-strategy/05-validacao-de-configuracao-e-repeticao.md`](10-testing-strategy/05-validacao-de-configuracao-e-repeticao.md) | Configuração inválida, validação no domínio, configuração extrema, meta já atingida, valor muito pequeno, múltiplas dosagens e repetição rápida. |
| [`10-testing-strategy/06-timers-memoria-e-camadas-de-teste.md`](10-testing-strategy/06-timers-memoria-e-camadas-de-teste.md) | Timers, navegação repetida, memória, contexto atual de configuração, testes de UI, testes do domínio e testes da abstração de hardware. |
| [`10-testing-strategy/07-simulador-cenarios-e-matriz.md`](10-testing-strategy/07-simulador-cenarios-e-matriz.md) | Simulador como ferramenta de testes, cenários (sensor parado, overshoot, sensor instável, dispenser parado, dispenser travado), máquina de estados e testes e matriz de cenários. |
| [`10-testing-strategy/08-regressao-ordem-e-hardware.md`](10-testing-strategy/08-regressao-ordem-e-hardware.md) | Testes de regressão, ordem recomendada de testes, testes antes do hardware, testes no hardware real, teste inicial do HX711, teste inicial do dispenser, teste físico sem ração, teste físico com pequenas quantidades e critério de segurança. |
| [`10-testing-strategy/09-logs-automacao-evolucao-e-principios.md`](10-testing-strategy/09-logs-automacao-evolucao-e-principios.md) | Logs durante testes, testes determinísticos, testes automatizados futuros, prioridade dos testes, implementação pronta, estratégia para agentes futuros, UI versus domínio, estado atual versus futuro, evolução esperada e princípio final. |

---

## Componentes-chave

- **Grupos de testes:** build, UI e navegação, domínio, hardware simulado e integração; futuramente hardware real.
- **Critério de sucesso:** `current_weight >= target_grams` → `dispenser.stop()` e `state = DOSING_STATE_COMPLETED`.
- **Estado atual:** dosagem normal nos dois modos (Massa e Valor R$), peso simulado crescente em duas etapas (rápida/fina), parada ao atingir a meta, interrupção (Parar/Emergência → tela Interrompida), liberação manual na Home, tara ao iniciar nova dosagem, conclusão já implementados.
- **Ainda planejado:** timeout, sensor sem progresso, sensor inválido, excesso de peso, tolerância de overshoot, estado `ERROR`, validações de domínio mais completas e testes automatizados.

---

## Documentos relacionados

- [`04-domain-and-state-machine.md`](04-domain-and-state-machine.md) — domínio e máquina de estados que orientam os testes.
- [`05-hardware-abstraction.md`](05-hardware-abstraction.md) — abstrações de hardware (`WeightSensor`/`Dispenser`) testadas nas camadas.
- [`06-simulation-strategy.md`](06-simulation-strategy.md) — simulação dos cenários de falha usados como ferramenta de testes.
- [`12-roadmap.md`](12-roadmap.md) — fases do projeto e a evolução da estratégia dentro do roadmap.

---

O arquivo original excedia 500 linhas e foi dividido em partes lógicas na subpasta `10-testing-strategy/`, preservando 100% do conteúdo factual.