# UI e Navegação

Documento canônico da **interface e da navegação** do dosador de ração (simulador desktop LVGL+SDL2 em C; futuro ESP32-S3): a responsabilidade de cada tela, o fluxo de navegação e a relação entre a UI e o domínio da aplicação.

A interface é construída com **LVGL** e, atualmente, executada no simulador desktop através de **SDL2** (no futuro, no display/touch do ESP32-S3).

A UI tem como responsabilidade principal:

* apresentar informações;
* receber ações do usuário;
* solicitar mudanças de estado;
* exibir o resultado do domínio.

Ela **não deve ser responsável pelas regras de dosagem**. A regra fundamental é:

> **A UI apresenta e solicita; o domínio decide; o hardware executa.**

Estrutura de navegação: Home → Seleção de modo → Configuração → Dosagem → Conclusão, com possibilidade de nova dosagem ou retorno ao início, além de cancelamento durante a dosagem.

Cada tela é criada quando solicitada pelo `Screen Manager`, que coordena tela atual, troca de telas e passagem de configuração (enum `Screen`); detalhes e navegação do ciclo completo estão nas partes abaixo.

Referências relacionadas: [04-domain-and-state-machine.md](04-domain-and-state-machine.md), [02-architecture.md](02-architecture.md).

O arquivo original excedia 500 linhas e foi dividido em partes lógicas na subpasta `07-ui-and-navigation/`, preservando 100% do conteúdo factual.

## Índice

- [01-objetivo-stack-e-screen-manager.md](07-ui-and-navigation/01-objetivo-stack-e-screen-manager.md) — Objetivo, objetivo da interface, stack LVGL/SDL2/ESP32-S3, organização de `src/ui/`, `ui_init()`, enum `Screen`, responsabilidade do Screen Manager e fluxo geral de navegação.
- [02-telas-home-e-selecao-de-modo.md](07-ui-and-navigation/02-telas-home-e-selecao-de-modo.md) — Tela Home, responsabilidades da Home, seleção de modo (quantidade fixa / porções) e voltar para o início.
- [03-tela-de-configuracao.md](07-ui-and-navigation/03-tela-de-configuracao.md) — Tela de configuração, quantidade fixa, porções, responsabilidade da configuração, `DosingConfig`, relação com a UI e botão Continuar.
- [04-tela-de-dosagem.md](07-ui-and-navigation/04-tela-de-dosagem.md) — Tela de dosagem, objetivo, peso atual, barra de progresso, fonte da verdade, timer, papel do timer, conclusão, cancelamento e independência do hardware.
- [05-tela-de-conclusao-e-mapa.md](07-ui-and-navigation/05-tela-de-conclusao-e-mapa.md) — Tela de conclusão, nova dosagem, voltar ao início após conclusão e mapa das telas.
- [06-estados-tela-e-responsabilidades.md](07-ui-and-navigation/06-estados-tela-e-responsabilidades.md) — Estados de tela versus estados do domínio, enumeração única, estado planejado, regra de responsabilidade, exemplos, comunicação da UI com o domínio, o que a UI não deve/pode fazer e configuração e validação.
- [07-ciclo-de-vida-e-plataformas.md](07-ui-and-navigation/07-ciclo-de-vida-e-plataformas.md) — Contexto da tela de configuração, ciclo de vida das telas, responsabilidade dos módulos, navegação sem regras de negócio, simulador, ESP32-S3, evolução visual, suporte a caracteres, interface 800×480 e touchscreen.
- [08-fluxos-do-usuario-e-principios.md](07-ui-and-navigation/08-fluxos-do-usuario-e-principios.md) — Fluxo completo do usuário, fluxos alternativos, estado de erro futuro, possível fluxo futuro de erro, regra para futuras telas, princípio de navegação, princípio de independência e princípio final.