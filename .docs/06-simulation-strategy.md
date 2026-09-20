# Simulation Strategy — Estratégia de Simulação

Documento canônico da **estratégia de simulação** do dosador (simulador desktop LVGL+SDL2 em C; futuro ESP32-S3). O simulador não é um protótipo descartável: é um ambiente de desenvolvimento para observar, desenvolver e testar o comportamento do dosador antes da disponibilidade e integração do hardware físico.

**Conteúdo canônico aqui:** o que simular (peso, liberação de ração, evolução da dosagem, tempo, falhas, condições anormais, conclusão e cancelamento) e os níveis de simulação:

- **N1 — Fluxo básico** (já implementado): dispenser ativo → peso +2 g → controller verifica → atingiu objetivo? → completa.
- **N2 — Fluxo temporal** (planejado): peso depende de `flow_rate × delta_time`.
- **N3 — Comportamento físico simplificado** (planejado): variação, atraso, overshoot, ruído.
- **N4 — Falhas** (planejado): sensor travado, sensor inválido, dispenser travado, fluxo insuficiente, ausência de progresso.
- **N5 — Cenários configuráveis** (planejado): selecionar diferentes condições de teste.

Cada nível se conecta aos componentes simulados `WeightSensor`/`Dispenser` definidos em [05-hardware-abstraction.md](05-hardware-abstraction.md) e reproduz comportamentos de estados do domínio descritos em [04-domain-and-state-machine.md](04-domain-and-state-machine.md).

O arquivo original excedia 500 linhas e foi dividido em partes lógicas na subpasta `06-simulation-strategy/`, preservando 100% do conteúdo factual.

## Índice

- [01-fundamentos-e-niveis.md](06-simulation-strategy/01-fundamentos-e-niveis.md) — Objetivo, por que simular antes do hardware, simulador como ambiente de desenvolvimento, o que deve ser simulado e os níveis de simulação N1–N5.
- [02-simulacao-atual-e-fisica.md](06-simulation-strategy/02-simulacao-atual-e-fisica.md) — Simulação atual do peso, por que começar simples, relação dispenser/peso, independência do domínio, taxa de alimentação, simulação por tempo, separação tempo/UI e ruído.
- [03-overshoot-fluxo-e-progresso.md](06-simulation-strategy/03-overshoot-fluxo-e-progresso.md) — Overshoot (configurável, não escondido), fluxo lento, ausência de fluxo e falta de progresso.
- [04-falhas-timeout-cancelamento-e-seguranca.md](06-simulation-strategy/04-falhas-timeout-cancelamento-e-seguranca.md) — Timeout, sensor travado, sensor com erro, dispenser travado, dispenser que não inicia, cancelamento e segurança em caso de erro.
- [05-cenarios-e-testes.md](06-simulation-strategy/05-cenarios-e-testes.md) — Cenários determinísticos, aleatórios, seed de simulação, cenários recomendados e simulação das porções.
- [06-arquitetura-da-simulacao.md](06-simulation-strategy/06-arquitetura-da-simulacao.md) — Independência da UI e do SDL2, relógio, o que não simular, fidelidade versus complexidade, contrato comportamental, testes de regressão, relação com o hardware real e arquitetura futura.
- [07-evolucao-e-principios.md](06-simulation-strategy/07-evolucao-e-principios.md) — Evolução incremental, critério para aumentar complexidade e os princípios finais (determinismo, isolamento, substituição, estado atual versus objetivo futuro, fluxo completo e princípios resumidos).