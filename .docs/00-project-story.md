Perfeito. Vou fazer o `00-project-story.md` como um documento de **contexto histórico + intenção arquitetural**, não apenas uma descrição do estado atual. A ideia é que um agente leia esse arquivo e entenda **como o projeto chegou à arquitetura atual e quais princípios não devem ser perdidos**.

# Project Story — Simulador de Dosador de Ração

## 1. Propósito deste documento

Este documento registra a história, a motivação e a evolução das decisões tomadas durante o desenvolvimento do projeto.

Seu objetivo principal é permitir que uma pessoa ou agente que entre no projeto posteriormente consiga entender não apenas **o que existe**, mas também:

* qual problema o projeto pretende resolver;
* por que o desenvolvimento começou no PC;
* por que existe uma camada de abstração de hardware;
* por que existem implementações simuladas;
* qual é o hardware final pretendido;
* quais decisões foram intencionais;
* quais partes são provisórias;
* qual direção o projeto deve seguir.

Este documento deve ser lido como **contexto do projeto**, e não como uma especificação detalhada de implementação.

---

# 2. Origem do projeto

O projeto surgiu a partir da ideia de desenvolver um protótipo de um **dosador automático de ração**.

O sistema final deverá ser capaz de receber uma configuração de dosagem, acionar um mecanismo físico para liberar ração e utilizar uma célula de carga para acompanhar a quantidade efetivamente liberada.

O hardware pretendido está detalhado em [08-target-hardware.md](08-target-hardware.md).

A ideia inicial poderia levar diretamente à implementação do firmware no ESP32.

Entretanto, isso faria com que problemas de interface, fluxo de aplicação e lógica de dosagem fossem descobertos somente depois que o hardware estivesse envolvido.

Por isso foi tomada uma decisão importante:

> **Desenvolver primeiro a aplicação em um simulador desktop utilizando LVGL + SDL2.**

Essa e as demais decisões arquiteturais estão registradas em [03-architecture-decisions.md](03-architecture-decisions.md).

---

# 3. Por que começar pelo PC

O simulador desktop permite desenvolver a aplicação sem depender inicialmente de:

* ESP32;
* display físico;
* touch;
* HX711;
* célula de carga;
* servo;
* alimentação;
* montagem mecânica.

Isso reduz o custo e a complexidade do ciclo inicial de desenvolvimento.

O objetivo não é criar uma versão descartável da aplicação.

O objetivo é utilizar o PC como ambiente de desenvolvimento e validação da maior parte da lógica que posteriormente será executada no dispositivo.

A estratégia de simulação (níveis e evolução pretendida) está descrita em [06-simulation-strategy.md](06-simulation-strategy.md).

Portanto, o simulador deve ser considerado parte do processo de desenvolvimento do produto, e não apenas uma demonstração visual.

---

# 4. Primeira versão da aplicação

A primeira aplicação era baseada no simulador de PC do próprio ecossistema LVGL.

O projeto utilizava:

* C;
* LVGL;
* SDL2;
* CMake;
* Visual Studio Build Tools;
* vcpkg.

Detalhes do ambiente de desenvolvimento no PC estão em [09-pc-development-environment.md](09-pc-development-environment.md).

O simulador permitiu executar a interface em uma janela desktop com a resolução pretendida para o display:

```text
800 × 480
```

Inicialmente havia código de demonstração do próprio LVGL.

Esse código foi desativado para que a aplicação passasse a representar o produto real.

---

# 5. Evolução da interface

A primeira versão funcional foi construída de forma incremental.

O fluxo inicial foi definido como `Home → Selecionar modo → Configurar dosagem → Dosagem`.

Posteriormente foi adicionada a conclusão (`Concluído`).

Também foram adicionadas ações de retorno e cancelamento para evitar que o usuário ficasse preso em uma tela.

O fluxo de navegação atual está detalhado em [07-ui-and-navigation.md](07-ui-and-navigation.md).

---

# 6. Modos de dosagem

Foram definidos inicialmente dois modos de configuração: **quantidade fixa** e **porções**.

As regras atuais de cada modo (limites, incrementos e evolução planejada) pertencem ao domínio e estão em [04-domain-and-state-machine.md](04-domain-and-state-machine.md).

A arquitetura deve permitir que essas regras evoluam sem exigir alterações profundas na interface ou no hardware.

---

# 7. Introdução do estado de configuração

Para evitar que cada tela mantivesse sua própria cópia dos dados, foi criado o conceito de configuração de dosagem (`DosingConfig`).

Essa configuração representa os dados do processo atual.

Ela é mantida pelo gerenciamento da aplicação e compartilhada com os componentes que precisam consultá-la.

A intenção é evitar que a UI seja responsável por armazenar isoladamente as regras e os valores do domínio.

A estrutura e o uso do `DosingConfig` estão detalhados em [02-architecture.md](02-architecture.md).

---

# 8. Problema identificado: UI não deveria controlar hardware

Durante a evolução da tela de dosagem, inicialmente a própria tela fazia algo equivalente a:

```text
timer da UI
    ↓
aumenta peso
    ↓
atualiza label
```

Isso funcionava para uma simulação simples, mas criava um problema arquitetural.

A tela passou a conhecer detalhes do comportamento físico.

Isso não seria adequado para o produto final.

Uma tela LVGL não deveria saber:

* como o HX711 funciona;
* como o servo é controlado;
* quanto tempo o servo permanece aberto;
* como o peso é convertido;
* como uma falha do sensor é detectada.

Essas responsabilidades pertencem a outras camadas ([02-architecture.md](02-architecture.md), [05-hardware-abstraction.md](05-hardware-abstraction.md)).

Essa percepção levou à criação do **Dosing Controller**.

---

# 9. Introdução do Dosing Controller

Foi criado um controlador de domínio responsável pelas regras da dosagem.

A intenção é separar interface, regras da dosagem e hardware.

O fluxo passou a ser:

```text
UI
 ↓
DosingController
 ↓
Hardware abstraction
```

A UI passou a consultar o controller para saber:

* peso atual;
* estado da dosagem;
* conclusão.

O controller passou a decidir:

* quando iniciar;
* quando continuar;
* quando parar;
* quando considerar a dosagem concluída;
* quando cancelar.

Essa separação é uma das decisões arquiteturais fundamentais do projeto ([02-architecture.md](02-architecture.md)).

---

# 10. Abstração do sensor de peso

Depois da criação do controller, foi criada uma abstração para o sensor de peso (`WeightSensor`).

A implementação atual é a `SimulatedWeightSensor`.

A implementação futura será baseada no HX711 + célula de carga.

A intenção é que o controller não precise saber se o peso vem de simulação ou de hardware real; ele deve trabalhar com a abstração.

A interface e as implementações estão detalhadas em [05-hardware-abstraction.md](05-hardware-abstraction.md).

---

# 11. Abstração do dispenser

O mesmo princípio foi aplicado ao mecanismo que libera a ração, resultando na abstração `Dispenser`.

A implementação atual é a `SimulatedDispenser`.

A implementação futura deverá controlar o mecanismo físico, inicialmente pensado em torno de um SG90 e/ou outro atuador apropriado ao mecanismo mecânico final.

Isso permite desenvolver a lógica da aplicação antes de definir todos os detalhes mecânicos.

A interface e as implementações estão detalhadas em [05-hardware-abstraction.md](05-hardware-abstraction.md).

---

# 12. Arquitetura resultante

A arquitetura evoluiu para uma separação conceitual: a **UI** (LVGL/telas/navegação) → o **domínio/controller** (DosingConfig, DosingController, DosingState) → a **interface de hardware** (WeightSensor, Dispenser), com implementações simuladas (PC/SDL2) e futuras (ESP32-S3, HX711/SG90).

O diagrama completo das camadas está em [02-architecture.md](02-architecture.md).

Essa separação deve ser preservada durante a evolução do projeto.

---

# 13. Por que a simulação não deve ser descartada

O código simulado não existe apenas porque o hardware ainda não está disponível.

Ele possui uma função importante no desenvolvimento.

Ele permite testar fluxo de dosagem, regras do controller, comportamento da UI, cancelamento, conclusão, tratamento de erros, diferentes velocidades de dosagem, comportamento próximo da meta e casos extremos ([06-simulation-strategy.md](06-simulation-strategy.md), [10-testing-strategy.md](10-testing-strategy.md)).

Isso também permite testar o domínio sem depender de um sensor físico.

Portanto:

> **A implementação simulada deve ser tratada como uma implementação legítima da abstração de hardware, e não como código descartável.**

---

# 14. Estado atual do projeto

Neste estágio, o simulador já possui LVGL e SDL2 funcionando em desktop, interface de 800×480, telas (Home, seleção de modo, configuração de quantidade/porções, dosagem, conclusão), controller de dosagem, sensor de peso e dispenser simulados, reset, cancelamento, barra de progresso, retorno para Home e nova dosagem.

O estado atual detalhado está em [12-roadmap.md](12-roadmap.md).

O fluxo principal já pode ser executado sem hardware físico.

---

# 15. O que ainda não deve ser considerado definitivo

Apesar de funcional, algumas partes ainda são deliberadamente simplificadas.

### Peso

Atualmente o peso é simulado diretamente em gramas.

O hardware real deverá trabalhar com uma leitura do HX711 e uma etapa de calibração.

Portanto, a simulação atual (`2 g → 4 g → 6 g → ...`) não representa ainda a leitura elétrica real da célula de carga.

A evolução da simulação do peso está em [06-simulation-strategy.md](06-simulation-strategy.md).

### Dispenser

Atualmente o dispenser possui apenas `start()`, `stop()` e `is_active()`; o comportamento mecânico real ainda não foi implementado.

O SG90 poderá exigir posição de abertura, posição de fechamento, temporização, ciclos, limites e comportamento mecânico específico.

Esses detalhes devem permanecer fora da lógica de negócio ([05-hardware-abstraction.md](05-hardware-abstraction.md), [08-target-hardware.md](08-target-hardware.md)).

### Máquina de estados

O projeto já possui estados relacionados à dosagem, mas a máquina de estados ainda deve evoluir para representar explicitamente todo o ciclo da aplicação, com direção pretendida para `IDLE → SELECT_MODE → CONFIGURING → DOSING → ERROR | COMPLETED`.

A definição final dos estados deverá ser feita antes da integração completa com o hardware ([04-domain-and-state-machine.md](04-domain-and-state-machine.md)).

---

# 16. Filosofia de desenvolvimento

O desenvolvimento segue uma estratégia incremental.

Cada etapa deve produzir algo executável, indo da UI básica à navegação, configuração, dosagem simulada, controller, hardware abstraído, simulação mais realista, tratamento de erros e, por fim, o hardware real.

As fases planejadas estão em [12-roadmap.md](12-roadmap.md).

A intenção é evitar construir todo o firmware e descobrir problemas somente quando o hardware estiver montado.

---

# 17. Princípio fundamental para futuros agentes

Um agente que modificar este projeto deve entender a seguinte regra:

> **Não acople a lógica de negócio diretamente ao LVGL ou ao hardware físico quando existir uma abstração apropriada.**

Por exemplo, a tela não deve passar a chamar diretamente algo como `hx711_read()` ou `servo_set_angle()`.

A preferência arquitetural é `UI → Controller → Abstração → Implementação`, o que mantém o simulador e o firmware conceitualmente alinhados.

As regras completas para agentes estão em [13-agent-guide.md](13-agent-guide.md).

---

# 18. Relação entre PC e ESP32

O objetivo final não é portar literalmente todo o código desktop para o ESP32.

O objetivo é preservar o máximo possível da lógica independente de plataforma: o domínio/controller (config, estado e regras) é compartilhado, enquanto a camada específica de plataforma (PC/SDL2 com simulados; ESP32-S3 com HX711/servo) pode mudar.

A lógica central deve mudar o mínimo possível.

O plano de migração está em [11-migration-pc-to-esp32.md](11-migration-pc-to-esp32.md).

---

# 19. Próxima direção

Depois do fluxo atual estar estável, o desenvolvimento deve seguir aproximadamente: consolidar a máquina de estados; tornar a simulação do dispenser mais realista; simular comportamento mais próximo de uma leitura de HX711; implementar calibração/conversão para gramas; adicionar falhas e timeouts; testar cenários de erro; finalizar as interfaces de hardware; iniciar integração com o ESP32-S3; integrar display e touch reais; integrar HX711 e célula de carga; integrar servo/atuador; validar o sistema físico completo.

As etapas e o roadmap estão em [12-roadmap.md](12-roadmap.md).

---

# 20. Estado conceitual do projeto

A visão final pode ser resumida assim: uma **aplicação** (UI em LVGL + domínio/controller) e um **hardware** (peso HX711 + dispenser SG90) integrados em torno do **ESP32-S3**.

O simulador PC existe para permitir que a maior parte dessa aplicação seja construída e validada **antes que essas dependências físicas estejam presentes**.

Esse é o principal motivo pelo qual a arquitetura atual utiliza interfaces de `WeightSensor` e `Dispenser` e mantém implementações simuladas separadas das futuras implementações de hardware.