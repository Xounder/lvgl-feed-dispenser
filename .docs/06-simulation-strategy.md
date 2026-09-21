# Simulation Strategy — Estratégia de Simulação

Documento canônico da **estratégia de simulação** do dosador (simulador desktop LVGL+SDL2 em C, em migração para C++; futuro ESP32-S3). O simulador não é um protótipo descartável: é um ambiente de desenvolvimento para observar, desenvolver e testar o comportamento do dosador antes da disponibilidade e integração do hardware físico.

**Conteúdo canônico aqui:** o que simular (peso, liberação de ração, evolução da dosagem em duas etapas, conversão monetária, liberação manual, tempo, falhas, condições anormais, conclusão, interrupção e cancelamento) e os níveis de simulação:

- **N1 — Fluxo básico** (já implementado): dispenser ativo → dosagem em duas etapas (rápida +20 g/tick enquanto faltam >30 g; fina +2 g/tick quando faltam ≤30 g) → controller verifica → atingiu objetivo? → completa. Modos Massa (g) e Valor (R$).
- **N2 — Fluxo temporal** (planejado): peso depende de `flow_rate × delta_time`.
- **N3 — Comportamento físico simplificado** (planejado): variação, atraso, overshoot, ruído.
- **N4 — Falhas** (planejado): sensor travado, sensor inválido, dispenser travado, fluxo insuficiente, ausência de progresso.
- **N5 — Cenários configuráveis** (planejado): selecionar diferentes condições de teste.

### Comportamento simulado já implementado

- **Dosagem em duas etapas (RP03/RS09):** na `dosing_controller_update()`, quando faltam mais de 30 g até a meta → fase rápida (`DOSING_PHASE_FAST`), adiciona +20 g a cada tick (~300 ms); quando faltam 30 g ou menos → fase fina (`DOSING_PHASE_FINE`), adiciona +2 g; ao atingir a meta o dispenser para e o estado vira `COMPLETED`. Pode ocorrer overshoot (o peso passa da meta antes do fechamento).
- **Modos Massa e Valor (R$):** no modo Massa a meta é informada em gramas; no modo Valor a meta é informada em reais e convertida em gramas pelo preço de referência por kg (padrão R$12,00/kg).
- **Liberação manual simulada (RS14/RS16):** botão "Liberacao manual" na Home que se mantém pressionado; enquanto ativo adiciona +5 g por tick (~200 ms) e o LED verde indica o modo manual; disponível somente no estado `IDLE` (bloqueada em `DOSING` — RS15).
- **Interrupção (RS11/RS12):** botões "Parar" e "Emergencia" na tela de Dosagem interrompem com prioridade → estado `INTERRUPTED`, massa parcial preservada.

Cada nível se conecta aos componentes simulados `WeightSensor`/`Dispenser` definidos em [05-hardware-abstraction.md](05-hardware-abstraction.md) e reproduz comportamentos de estados do domínio descritos em [04-domain-and-state-machine.md](04-domain-and-state-machine.md).

O arquivo original excedia 500 linhas e foi dividido em partes lógicas na subpasta `06-simulation-strategy/`, preservando 100% do conteúdo factual.

## Índice

- [01-fundamentos-e-niveis.md](06-simulation-strategy/01-fundamentos-e-niveis.md) — Objetivo, por que simular antes do hardware, simulador como ambiente de desenvolvimento, o que deve ser simulado e os níveis de simulação N1–N5.
- [02-simulacao-atual-e-fisica.md](06-simulation-strategy/02-simulacao-atual-e-fisica.md) — Simulação atual do peso, por que começar simples, relação dispenser/peso, independência do domínio, taxa de alimentação, simulação por tempo, separação tempo/UI e ruído.
- [03-overshoot-fluxo-e-progresso.md](06-simulation-strategy/03-overshoot-fluxo-e-progresso.md) — Overshoot (configurável, não escondido), fluxo lento, ausência de fluxo e falta de progresso.
- [04-falhas-timeout-cancelamento-e-seguranca.md](06-simulation-strategy/04-falhas-timeout-cancelamento-e-seguranca.md) — Timeout, sensor travado, sensor com erro, dispenser travado, dispenser que não inicia, cancelamento, interrupção/emergência e segurança em caso de erro.
- [05-cenarios-e-testes.md](06-simulation-strategy/05-cenarios-e-testes.md) — Cenários determinísticos, aleatórios, seed de simulação, cenários recomendados, conversão monetária (modo Valor R$) e simulação da liberação manual.
- [06-arquitetura-da-simulacao.md](06-simulation-strategy/06-arquitetura-da-simulacao.md) — Independência da UI e do SDL2, relógio, o que não simular, fidelidade versus complexidade, contrato comportamental, testes de regressão, relação com o hardware real e arquitetura futura.
- [07-evolucao-e-principios.md](06-simulation-strategy/07-evolucao-e-principios.md) — Evolução incremental, critério para aumentar complexidade e os princípios finais (determinismo, isolamento, substituição, estado atual versus objetivo futuro, fluxo completo e princípios resumidos).