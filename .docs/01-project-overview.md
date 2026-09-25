# Project Overview — Simulador de Dosador de Ração

## 1. Visão geral

Este projeto é o desenvolvimento de um sistema de **dosagem automática de ração**, inicialmente desenvolvido e validado em um ambiente desktop por meio de um simulador baseado em **LVGL + SDL2**, com posterior migração para um dispositivo físico baseado em **ESP32-S3**.

O sistema tem como objetivo permitir que o usuário configure uma dosagem e acompanhe o processo enquanto o equipamento libera ração e monitora seu peso por meio de uma célula de carga.

A arquitetura foi projetada para separar:

* interface gráfica;
* regras de negócio;
* abstrações de hardware;
* implementações de hardware;
* implementações simuladas.

Essa separação permite desenvolver e validar a aplicação antes da disponibilidade do hardware físico.

---

# 2. Objetivo do sistema

O objetivo final é construir um dispositivo capaz de:

1. apresentar uma interface gráfica ao usuário;
2. permitir a escolha do modo de dosagem;
3. permitir a configuração da quantidade desejada;
4. acionar o mecanismo responsável pela liberação da ração;
5. medir continuamente o peso da ração;
6. interromper a liberação quando a quantidade desejada for atingida;
7. informar o resultado ao usuário;
8. detectar e tratar situações anormais.

O sistema deve funcionar como um equipamento embarcado independente, com interface própria e sensores/atuadores físicos.

---

# 3. Produto final pretendido

O produto final será baseado em um **ESP32-S3** conectado aos periféricos necessários para formar o dosador.

A arquitetura geral pretendida segue a direção `Display + Touch (LVGL) → Dosing Controller → Sensor de peso (HX711 → célula de carga) | Dispenser/Atuador (SG90)`.

O detalhamento das camadas está em [02-architecture.md](02-architecture.md) e o hardware associado em [08-target-hardware.md](08-target-hardware.md).

---

# 4. Hardware alvo

O hardware considerado para a primeira versão do produto (ESP32-S3, display LCD 4,3" 800×480, touch capacitivo, HX711, célula de carga, SG90/atuador, reservatório, botões físicos, indicador, fonte 5 V e estrutura física) está detalhado em [08-target-hardware.md](08-target-hardware.md).

A implementação mecânica do mecanismo de dosagem ainda poderá sofrer alterações conforme os testes físicos.

Portanto, o software não deve assumir detalhes mecânicos que ainda não foram validados.

---

# 5. Ambiente de desenvolvimento atual

Antes da integração com o ESP32-S3, o sistema é desenvolvido em um ambiente desktop.

A implementação atual utiliza **C** (em migração para **C++**), **LVGL**, **SDL2**, **CMake**, **vcpkg** e Visual Studio Build Tools no Windows. Para o ESP32-S3 o alvo é **C++ com Arduino framework via Arduino IDE (core ESP32)**.

O desktop funciona como uma plataforma de desenvolvimento e simulação.

A aplicação é executada em uma janela com a área útil do display alvo,
que é um LCD 4,3" 800×480 usado em **orientação retrato**:

```text
480 × 800 (área útil da UI)
```

Isso permite testar a interface em uma proporção próxima à do hardware final.

Os detalhes de ambiente, build e execução estão em [09-pc-development-environment.md](09-pc-development-environment.md).

---

# 6. Estrutura lógica atual

O código está organizado principalmente em três áreas (`src/`):

* `domain/` — lógica do comportamento do sistema: configuração da dosagem, estado do processo e regras para iniciar, atualizar, cancelar e concluir. Ver [04-domain-and-state-machine.md](04-domain-and-state-machine.md).
* `hardware/` — abstrações `WeightSensor` e `Dispenser` com implementações simuladas, substituíveis no futuro por HX711 + célula de carga e SG90/atuador sem que o controller precise conhecê-los. Ver [05-hardware-abstraction.md](05-hardware-abstraction.md).
* `ui/` — telas (Home com liberação manual, LED e seleção de modo; Configuração; Dosagem com `INTERROMPER DOSAGEM`; Interrompido; Concluído) e navegação via `screen_manager`. Ver [07-ui-and-navigation.md](07-ui-and-navigation.md).

A UI deve permanecer responsável principalmente por apresentar informações, receber interação do usuário, solicitar ações ao domínio e refletir o estado atual da aplicação.

Ela não deve assumir o papel de controlador do hardware.

A estrutura completa do `src/` está em [02-architecture.md](02-architecture.md).

---

# 7. Fluxo atual da aplicação

O fluxo principal implementado é: `Home (seleção de modo: Massa | Valor R$) → Configuração → Dosando → (Interromper/Emergência física → Interrompido → Nova dosagem) ou (Meta atingida → Concluído → Nova dosagem)`.

As telas de conclusão e de interrupção permitem iniciar uma nova dosagem ou voltar ao início.

O fluxo de navegação detalhado está em [07-ui-and-navigation.md](07-ui-and-navigation.md).

---

# 8. Modos de dosagem

Existem dois modos de dosagem:

* **Massa** — o usuário informa diretamente a quantidade de ração desejada em gramas (`target_grams`);
* **Valor (R$)** — o usuário informa o valor desejado em centavos (`target_money_cents`); o sistema converte para gramas usando o preço de referência por unidade de massa (`price_per_kg_cents`, padrão R$ 12,00/kg).

No modo Valor, a conversão segue o requisito RS05: `gramas = (target_money_cents * 1000) / price_per_kg_cents`. O `DosingConfig` é `{ DosingMode mode; int target_grams; int target_money_cents; int price_per_kg_cents; }`, com `DosingMode = DOSING_MODE_GRAMS | DOSING_MODE_CURRENCY`.

As regras de cada modo (incrementos, limites e conversão valor → massa) pertencem ao domínio e estão em [04-domain-and-state-machine.md](04-domain-and-state-machine.md).

---

# 9. Funcionamento da simulação atual

O simulador representa o hardware por meio de implementações artificiais.

Quando uma dosagem é iniciada, o controller reseta o peso, inicia o dispenser, o sensor simulado começa em 0 g e o peso aumenta gradualmente. A liberação ocorre em duas etapas (`DosingPhase`): etapa **rápida** com vazão alta (+20 g a cada 300 ms) enquanto faltam mais de 30 g, e etapa **fina** com vazão reduzida (+2 g) quando faltam 30 g ou menos. Ao atingir a meta, o dispenser para e o estado passa a `COMPLETED`.

Na implementação atual, a simulação adiciona uma pequena quantidade de peso a cada atualização do processo (300 ms).

Isso não pretende representar ainda o comportamento físico exato da ração; seu objetivo atual é permitir validar o fluxo de aplicação.

A estratégia de simulação está em [06-simulation-strategy.md](06-simulation-strategy.md).

---

# 10. Abstração do sensor de peso

O sistema utiliza a interface conceitual `WeightSensor`.

No PC, a implementação é simulada; no dispositivo final, a intenção é `Célula de carga → HX711 → Implementação WeightSensor → DosingController`.

A lógica de dosagem não deve depender diretamente do HX711.

A interface e as implementações estão em [05-hardware-abstraction.md](05-hardware-abstraction.md).

---

# 11. Abstração do dispenser

O mecanismo de liberação é representado pela interface `Dispenser`.

No simulador existe a `SimulatedDispenser`; no hardware, um servo/atuador.

O controller apenas solicita operações ao dispenser; ele não deve conhecer GPIO, PWM, posição do servo ou temporização elétrica.

Esses detalhes pertencem à implementação de hardware ([05-hardware-abstraction.md](05-hardware-abstraction.md)).

---

# 12. Estados da aplicação

O `DosingState` atual implementa os estados `IDLE`, `DOSING`, `COMPLETED` e `INTERRUPTED`. `cancel()` / emergência (botões "Emergência"/"Parar") levam ao estado `INTERRUPTED`, preservando a massa parcial e priorizando a parada; `new_dosing()` realiza a tara/reset e retorna ao `IDLE`.

A direção arquitetural planejada é representar explicitamente o ciclo completo: `IDLE → SELECT_MODE → CONFIGURING → DOSING → ERROR | COMPLETED`, restando `SELECT_MODE`, `CONFIGURING` e `ERROR` como evolução prevista (não fazem parte do enum atual).

Os estados representam o **comportamento do domínio**, e não simplesmente as telas do LVGL.

Uma tela pode representar um estado, mas estado e tela não devem ser considerados necessariamente a mesma coisa.

Essa distinção será importante conforme o projeto evoluir ([04-domain-and-state-machine.md](04-domain-and-state-machine.md)).

---

# 13. Separação entre UI, domínio e hardware

Uma das principais características do projeto é a separação de responsabilidades, na direção `UI (LVGL/SDL2) → DOMAIN (DosingController, DosingConfig, DosingState) → HARDWARE ABSTRACTION (WeightSensor, Dispenser) → Simulado | Real`.

Essa divisão é importante porque o mesmo domínio deve poder ser executado no PC e posteriormente no ESP32-S3, com diferentes implementações de hardware.

O detalhamento está em [02-architecture.md](02-architecture.md).

---

# 14. O que pertence ao simulador

O simulador deve fornecer um ambiente suficientemente próximo do comportamento do dispositivo para permitir validar interface, navegação, configuração, fluxo de dosagem, regras do controller, estados, cancelamento, conclusão, tratamento de erros e o comportamento de sensores e atuadores simulados.

O simulador não precisa reproduzir perfeitamente a física do dispositivo.

O nível de fidelidade deve aumentar conforme isso trouxer valor para a validação do sistema ([06-simulation-strategy.md](06-simulation-strategy.md)).

---

# 15. O que pertence ao hardware real

Quando o projeto for migrado para o ESP32-S3, deverão ser substituídas ou adicionadas implementações específicas para: display físico 800×480, touch capacitivo, peso real (célula de carga + HX711, com leitura, tara, calibração, conversão, filtragem e tratamento de leituras inválidas), dispenser (servo/atuador) e entradas físicas (botões e demais controles).

Detalhes em [08-target-hardware.md](08-target-hardware.md) e [11-migration-pc-to-esp32.md](11-migration-pc-to-esp32.md).

---

# 16. Objetivo da migração

A migração para o ESP32-S3 não deve representar uma reescrita completa da aplicação.

A intenção é preservar o máximo possível das camadas independentes de hardware: o domínio é compartilhado entre o PC (LVGL/SDL2 + hardware simulado) e o ESP32-S3 (LVGL + hardware real).

O que muda principalmente são as camadas dependentes da plataforma ([11-migration-pc-to-esp32.md](11-migration-pc-to-esp32.md)).

---

# 17. Objetivo de longo prazo

O resultado esperado é um sistema embarcado capaz de realizar uma dosagem real de ração com comportamento previsível e controlado.

A evolução pretendida vai do simulador funcional → simulação realista → testes de falha → integração ESP32-S3 → sensores/atuadores reais → protótipo físico.

As fases estão detalhadas em [12-roadmap.md](12-roadmap.md).

---

# 18. Princípios que devem permanecer

Independentemente das futuras mudanças de implementação, os seguintes princípios devem ser preservados:

### 1. Simulação antes do hardware

Sempre que possível, validar a lógica no PC antes de depender do dispositivo físico.

### 2. Domínio independente da UI

As regras de dosagem não devem ser implementadas dentro dos callbacks das telas.

### 3. Domínio independente do hardware

O `DosingController` não deve depender diretamente de HX711, GPIO ou servo.

### 4. Hardware atrás de abstrações

Sensores e atuadores devem possuir interfaces que permitam implementações diferentes.

### 5. Evolução incremental

Mudanças grandes devem ser divididas em etapas pequenas e verificáveis.

### 6. Não confundir protótipo com arquitetura descartável

Mesmo que algumas implementações atuais sejam simplificadas, elas devem respeitar a direção arquitetural definida.

### 7. O simulador faz parte do processo de desenvolvimento

Ele deve continuar útil mesmo depois que o hardware físico existir.

---

# 19. Estado do projeto

No momento, o projeto encontra-se na fase `DESENVOLVIMENTO DO SIMULADOR`.

O fluxo básico da aplicação já está funcional.

As próximas evoluções (consolidar a máquina de estados, melhorar a simulação física, simular o sensor de peso de maneira mais realista, adicionar cenários de erro, definir melhor as interfaces finais de hardware e iniciar a integração com o ESP32-S3) estão detalhadas em [12-roadmap.md](12-roadmap.md).

O projeto ainda não deve considerar a implementação física como finalizada.

---

# 20. Resumo

Em uma frase:

> **Este projeto é uma aplicação de dosagem automática de ração desenvolvida primeiro em um simulador desktop com LVGL, utilizando uma arquitetura que separa UI, domínio e hardware para permitir que a mesma lógica evolua posteriormente para um dispositivo físico baseado em ESP32-S3.**

O principal objetivo arquitetural é fazer com que a aplicação possa evoluir de `Simulação` para `Protótipo físico` sem precisar abandonar a estrutura e a lógica desenvolvidas durante a fase de software.

---

# 21. Rastreabilidade dos requisitos do trabalho acadêmico (Trabalho.md)

A tabela abaixo mapeia os requisitos físicos (RP01–RP08) e de software (RS01–RS16) do trabalho acadêmico para a documentação canônica correspondente e o status atual de cada um.

Estado: **Implementado** = presente e funcional no simulador; **Planejado** = documentado como evolução futura; **Hardware físico** = depende da montagem/migração para o ESP32-S3.

| Código | Requisito (texto resumido do Trabalho.md) | Cobertura na documentação | Status |
| --- | --- | --- | --- |
| RP01 | Reservatório para armazenamento do produto | [08-target-hardware.md](08-target-hardware.md) | Hardware físico (planejado) |
| RP02 | Permitir abastecimento e reabastecimento | [08-target-hardware.md](08-target-hardware.md) | Hardware físico (planejado) |
| RP03 | Mecanismo para liberar/interromper fisicamente a liberação (etapas rápida e fina) | [04-domain-and-state-machine.md](04-domain-and-state-machine.md), [06-simulation-strategy.md](06-simulation-strategy.md), [08-target-hardware.md](08-target-hardware.md) | Etapas rápida/fina implementadas no simulador; mecanismo físico planejado |
| RP04 | Região para posicionamento do recipiente (plataforma) | [08-target-hardware.md](08-target-hardware.md) | Hardware físico (planejado) |
| RP05 | Pesagem do produto efetivamente recebido (4 células de carga de 5 kg nos 4 pontos de apoio) | [06-simulation-strategy.md](06-simulation-strategy.md), [08-target-hardware.md](08-target-hardware.md), [10-testing-strategy.md](10-testing-strategy.md) | Hardware físico (planejado); pesagem simulada no PC |
| RP06 | Permitir visualizar e demonstrar o processo físico | [08-target-hardware.md](08-target-hardware.md) | Hardware físico (planejado) |
| RP07 | Permitir retirar o produto e reposicionar/substituir o recipiente | [08-target-hardware.md](08-target-hardware.md) | Hardware físico (planejado); tara implementada no simulador |
| RP08 | Dispositivo geral de liga/desliga (chave geral) | [08-target-hardware.md](08-target-hardware.md) | Hardware físico (planejado) |
| RS01 | Selecionar dosagem por massa ou valor monetário | [04-domain-and-state-machine.md](04-domain-and-state-machine.md), [07-ui-and-navigation.md](07-ui-and-navigation.md) | Implementado (modos Massa e Valor R$) |
| RS02 | Informar a massa desejada | [04-domain-and-state-machine.md](04-domain-and-state-machine.md), [07-ui-and-navigation.md](07-ui-and-navigation.md) | Implementado |
| RS03 | Informar o valor monetário desejado | [04-domain-and-state-machine.md](04-domain-and-state-machine.md), [07-ui-and-navigation.md](07-ui-and-navigation.md) | Implementado |
| RS04 | Utilizar preço de referência por unidade de massa | [04-domain-and-state-machine.md](04-domain-and-state-machine.md), [08-target-hardware.md](08-target-hardware.md) | Implementado (padrão R$ 12,00/kg) |
| RS05 | Determinar a massa correspondente ao valor informado | [04-domain-and-state-machine.md](04-domain-and-state-machine.md) | Implementado (gramas = valor×1000/preço-por-kg) |
| RS06 | Iniciar automaticamente a liberação após o comando | [04-domain-and-state-machine.md](04-domain-and-state-machine.md), [06-simulation-strategy.md](06-simulation-strategy.md) | Implementado |
| RS07 | Monitorar a massa recebida durante a dosagem | [06-simulation-strategy.md](06-simulation-strategy.md), [10-testing-strategy.md](10-testing-strategy.md) | Implementado (simulado) |
| RS08 | Apresentar visualmente a massa atualizada | [07-ui-and-navigation.md](07-ui-and-navigation.md) | Implementado |
| RS09 | Controlar e interromper automaticamente a liberação (rápida/fina + fechamento) | [04-domain-and-state-machine.md](04-domain-and-state-machine.md), [06-simulation-strategy.md](06-simulation-strategy.md) | Implementado |
| RS10 | Apresentar estados Aguardando, Dosando e Concluído | [04-domain-and-state-machine.md](04-domain-and-state-machine.md), [07-ui-and-navigation.md](07-ui-and-navigation.md) | Implementado (IDLE, DOSING, COMPLETED) |
| RS11 | Interrupção manual da dosagem (tela "Parar" / botão físico) | [04-domain-and-state-machine.md](04-domain-and-state-machine.md), [07-ui-and-navigation.md](07-ui-and-navigation.md), [08-target-hardware.md](08-target-hardware.md) | Implementado (botões "Parar"/"Emergência" → INTERRUPTED) |
| RS12 | Prioridade da interrupção manual | [04-domain-and-state-machine.md](04-domain-and-state-machine.md) | Implementado |
| RS13 | Encerrar a operação e preparar nova dosagem (tara) | [04-domain-and-state-machine.md](04-domain-and-state-machine.md), [07-ui-and-navigation.md](07-ui-and-navigation.md) | Implementado (`new_dosing()` → tara → IDLE) |
| RS14 | Permitir liberação manual direta | [04-domain-and-state-machine.md](04-domain-and-state-machine.md), [07-ui-and-navigation.md](07-ui-and-navigation.md), [08-target-hardware.md](08-target-hardware.md) | Implementado em IDLE (botão "Liberação manual") |
| RS15 | Impedir liberação manual durante dosagem automática | [04-domain-and-state-machine.md](04-domain-and-state-machine.md), [07-ui-and-navigation.md](07-ui-and-navigation.md) | Implementado (bloqueada fora do estado IDLE) |
| RS16 | Indicar visualmente o modo de liberação manual (LED) | [07-ui-and-navigation.md](07-ui-and-navigation.md), [08-target-hardware.md](08-target-hardware.md) | Implementado (LED indicador verde na Home); LED físico planejado |