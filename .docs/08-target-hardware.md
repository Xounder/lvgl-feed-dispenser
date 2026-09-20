# Hardware Alvo

> **Documento canônico** de **hardware alvo** do dosador de ração.
> Este arquivo é o índice; o conteúdo completo está nas partes listadas abaixo.
> Outras partes da documentação devem linkar para este arquivo ao tratar do hardware físico planejado: ESP32-S3, display/touch, HX711/load cell, servo/mecanismo, botões, indicadores, alimentação e física do dispositivo.

---

## Resumo

Este documento descreve o hardware físico planejado para a versão final do projeto de dosagem automática de ração.

O sistema está sendo desenvolvido inicialmente em um simulador desktop para validar a interface, o fluxo da aplicação, a lógica de dosagem e as abstrações de hardware.

O hardware físico será responsável por transformar esse comportamento simulado em um dispositivo real.

A plataforma alvo é baseada em:

```text
ESP32-S3
   │
   ├── Display 4,3" — 800×480
   ├── Touch capacitivo
   ├── HX711
   ├── Load Cell
   ├── SG90 / atuador
   ├── Botões físicos
   ├── Chave liga/desliga
   ├── LED / indicador
   ├── Reservatório
   ├── Mecanismo de dosagem
   └── Fonte de alimentação
```

O ESP32-S3 será o controlador central do dispositivo, responsável por executar a aplicação, a lógica de dosagem, o LVGL, a comunicação com display e touch, o HX711, o servo/atuador, os botões, o gerenciamento de estados e o tratamento de erros.

---

## Índice

| Parte | Conteúdo |
| ----- | -------- |
| [01-objetivo-e-visao-geral.md](08-target-hardware/01-objetivo-e-visao-geral.md) | Objetivo, visão geral do dispositivo, ESP32-S3 e seu papel na arquitetura física |
| [02-display-e-touch.md](08-target-hardware/02-display-e-touch.md) | Display 4,3" 800×480, razão para desenvolver na resolução final, display/LVGL e touch capacitivo (com abstração) |
| [03-sensor-de-peso.md](08-target-hardware/03-sensor-de-peso.md) | HX711, load cell, montagem mecânica, peso do recipiente, tara, calibração, precisão e filtragem do peso |
| [04-dispenser-servo-e-mecanismo.md](08-target-hardware/04-dispenser-servo-e-mecanismo.md) | SG90, distinção servo vs. dispenser, possível mecanismo, posição do servo, inércia e overshoot |
| [05-botoes-e-indicadores.md](08-target-hardware/05-botoes-e-indicadores.md) | Botões físicos, debounce, chave liga/desliga e indicador/LED |
| [06-alimentacao.md](08-target-hardware/06-alimentacao.md) | Alimentação 5 V, picos do servo, separação de alimentação, MB102 e protoboard |
| [07-reservatorio-e-mecanica.md](08-target-hardware/07-reservatorio-e-mecanica.md) | Reservatório, recipiente de pesagem e relação entre mecânica e software |
| [08-integracao-e-fluxos.md](08-target-hardware/08-integracao-e-fluxos.md) | Fluxo físico completo e integrações lógicas do sensor, dispenser, botões, indicadores e comunicação entre componentes |
| [09-abstracao-e-dependencias.md](08-target-hardware/09-abstracao-e-dependencias.md) | Tabela de dependências físicas, componentes simulados atualmente e o que não deve ir para o domínio |
| [10-validacao-incremental.md](08-target-hardware/10-validacao-incremental.md) | Primeira integração física, validação incremental de cada subsistema, primeira dosagem, overshoot e relação com a simulação |
| [11-limitacoes-e-seguranca.md](08-target-hardware/11-limitacoes-e-seguranca.md) | Limitações conhecidas, pinagem, segurança elétrica e mecânica, e protótipo versus produto final |
| [12-criterio-de-sucesso-e-roadmap.md](08-target-hardware/12-criterio-de-sucesso-e-roadmap.md) | Critério de sucesso da integração, relação com o projeto atual, estado atual do hardware, roadmap e princípio final |

---

## Documentos relacionados

- [05-hardware-abstraction.md](05-hardware-abstraction.md) — abstrações de hardware (`WeightSensor`, `Dispenser` e implementações)
- [11-migration-pc-to-esp32.md](11-migration-pc-to-esp32.md) — migração do simulador PC para o ESP32-S3
- [02-architecture.md](02-architecture.md) — arquitetura e responsabilidades das camadas
- [04-domain-and-state-machine.md](04-domain-and-state-machine.md) — domínio, estados e regras da dosagem
- [06-simulation-strategy.md](06-simulation-strategy.md) — estratégia de simulação