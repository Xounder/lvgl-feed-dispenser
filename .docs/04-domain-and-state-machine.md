# Domain and State Machine — Domínio e Máquina de Estados

> **Documento canônico** de **domínio e máquina de estados** do dosador de ração.
> Este arquivo é o índice; o conteúdo completo está nas partes listadas abaixo.
> Outras partes da documentação devem linkar para este arquivo ao tratar de estados, transições, eventos, regras de dosagem, configuração, cancelamento, conclusão, erros e evolução da máquina de estados.

---

## Resumo

Este documento define o comportamento do domínio do dosador de ração e sua máquina de estados.

Ele descreve:

* estados da aplicação;
* significado de cada estado;
* transições;
* eventos que provocam transições;
* regras da dosagem;
* configuração;
* cancelamento;
* conclusão;
* erros;
* comportamento esperado do controller.

O objetivo é fornecer uma referência para a implementação atual e para futuras evoluções do sistema.

---

## Princípio central

O estado do domínio representa o **processo real de dosagem**, e não simplesmente a tela que está sendo exibida.

A máquina de estados deve evoluir conforme os requisitos reais forem definidos, e não através da criação antecipada de estados sem comportamento associado.

---

## Índice

| Parte | Conteúdo |
| ----- | -------- |
| [`04-domain-and-state-machine/01-fundamentos-e-principio.md`](04-domain-and-state-machine/01-fundamentos-e-principio.md) | Objetivo, princípio do domínio (estado ≠ tela), máquina de estados geral e estados atuais (`DosingState`). |
| [`04-domain-and-state-machine/02-estados-e-configuracao.md`](04-domain-and-state-machine/02-estados-e-configuracao.md) | Estados `SELECT_MODE`/`CONFIGURING` (evolução prevista), modos (Massa e Valor R$) e início da dosagem. |
| [`04-domain-and-state-machine/03-regras-da-dosagem.md`](04-domain-and-state-machine/03-regras-da-dosagem.md) | Regras de dosagem (fases rápida/fina), conclusão, interrupção (estado `INTERRUPTED`), cancelamento e estado `ERROR`. |
| [`04-domain-and-state-machine/04-tabelas-eventos-e-invariantes.md`](04-domain-and-state-machine/04-tabelas-eventos-e-invariantes.md) | Tabela de estados, eventos, relação eventos-estados e regras invariantes. |
| [`04-domain-and-state-machine/05-regras-de-borda.md`](04-domain-and-state-machine/05-regras-de-borda.md) | Timeout, falta de progresso, overshoot, controle do dispenser, leitura do peso e atualização do processo. |
| [`04-domain-and-state-machine/06-responsabilidades-e-evolucao.md`](04-domain-and-state-machine/06-responsabilidades-e-evolucao.md) | Responsabilidades (tela, controller, UI, hardware), estado atual da implementação, evolução planejada e princípios finais. |

---

## Componentes-chave

- `DosingState` — enum atual com `DOSING_STATE_IDLE`, `DOSING_STATE_DOSING`, `DOSING_STATE_COMPLETED`, `DOSING_STATE_INTERRUPTED`; estados `SELECT_MODE`, `CONFIGURING` e `ERROR` são evolução prevista.
- `DosingConfig` — `mode` (`DOSING_MODE_GRAMS` ou `DOSING_MODE_CURRENCY`), `target_grams`, `target_money_cents` e `price_per_kg_cents`.
- `DosingPhase` — fases da dosagem: `DOSING_PHASE_FAST` e `DOSING_PHASE_FINE`.
- `DosingController` — centraliza início, atualização, interrupção, conclusão, liberação manual e conversão do modo Valor.

Máquina de estados (visão geral):

```text
        IDLE ── start ──► DOSING ── meta atingida ──► COMPLETED
          ▲                │
          │   cancelar /   │
          │   emergência   ▼
          │          INTERRUPTED
          │
          └── «new_dosing» (tara) ── volta a IDLE
              (idem a partir de COMPLETED)
```

O estado `INTERRUPTED` preserva a massa parcial; a nova dosagem (`new_dosing`) executa tara/reset e retorna a `IDLE`. `SELECT_MODE`, `CONFIGURING` e `ERROR` permanecem como evolução prevista.

A abstração de hardware usada pelo domínio é tratada em [`05-hardware-abstraction.md`](05-hardware-abstraction.md).