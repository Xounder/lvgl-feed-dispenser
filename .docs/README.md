# Dosador de Ração

Simulador desktop de um sistema de dosagem automática de ração, desenvolvido em **C + LVGL + SDL2**, com arquitetura preparada para futura execução em um **ESP32-S3** com display, touch, célula de carga, HX711 e dispenser físico.

O projeto segue uma abordagem **simulation-first**: desenvolver e validar o comportamento da aplicação no PC antes de integrar o hardware real.

---

## 📖 História

O projeto começou como uma proposta de sistema embarcado para dosagem automática de ração.

Como o hardware físico ainda não estava disponível, a primeira etapa foi construir um simulador desktop capaz de reproduzir a interface e o comportamento esperado do dispositivo.

A arquitetura foi sendo organizada para separar **UI → Domínio → Abstrações de hardware → Implementações simuladas/reais**, para que a lógica desenvolvida no simulador possa ser aproveitada durante a migração para o ESP32-S3.

Mais detalhes sobre a evolução e as decisões tomadas estão em:

→ [`00-project-story.md`](00-project-story.md)

---

## 🎯 Visão geral

O sistema permite configurar uma dosagem e acompanhar sua execução, seguindo o fluxo **Home → Seleção de modo → Configuração → Dosagem → (Conclusão | Interrupção)**.

Existem atualmente dois modos de dosagem:

- **Massa (gramas)** — o usuário informa diretamente a massa desejada em gramas.
- **Valor (R$)** — o usuário informa o valor em centavos; o sistema converte para gramas usando o preço de referência por unidade de massa (padrão R$ 12,00/kg).

No simulador, o peso e o dispenser são representados por implementações de hardware simuladas.

Visão detalhada:

→ [`01-project-overview.md`](01-project-overview.md)

---

## 🏗️ Arquitetura

A arquitetura foi pensada para separar responsabilidades em camadas:

```text
UI (LVGL)
    ↓
Domain (DosingController)
    ↓
Hardware Interfaces (WeightSensor, Dispenser)
    ↓
Implementações Simuladas / Reais
```

A ideia central é:

> **A UI apresenta e solicita; o domínio decide; o hardware executa.**

Documentação:

- [`02-architecture.md`](02-architecture.md)
- [`03-architecture-decisions.md`](03-architecture-decisions.md)
- [`04-domain-and-state-machine.md`](04-domain-and-state-machine.md)

---

## 📊 Estado atual

O simulador desktop está funcional: o ciclo completo — configuração (Massa / Valor R$), dosagem em etapas rápida/fina, conclusão, interrupção (Parar/Emergência), tara, liberação manual e nova dosagem — já pode ser executado no PC, com peso e dispenser simulados.

A evolução imediata está concentrada na **fase de robustez**: tratamento de erros, timeout, simulação de falhas e testes mais robustos, antes da migração para o ESP32-S3.

Roadmap completo:

→ [`12-roadmap.md`](12-roadmap.md)

---

## 💻 Como executar

### Requisitos

Ambiente atualmente utilizado:

```text
Windows
Visual Studio Build Tools
MSVC
CMake
vcpkg
SDL2
LVGL
```

### Passos rápidos

Na raiz do projeto (`pc-vscode`):

```powershell
cmake --build build --config Debug
.\bin\Debug\main.exe
```

O executável é gerado em `bin/Debug/main.exe`. No ambiente atual, a DLL de debug do SDL2 (`SDL2d.dll`) deve estar disponível junto ao executável.

Detalhes (configuração via CMake com vcpkg, toolchain, `SDL2d.dll` e troubleshooting):

- → [`09-pc-development-environment.md`](09-pc-development-environment.md)
- → [`run-code.md`](run-code.md)

---

## 🧪 Testes e simulação

O simulador permite validar o fluxo principal sem depender do hardware físico:

```text
Dispenser ativo → Peso aumenta → Meta atingida → Dispenser para → Dosagem concluída
```

Também existe suporte à interrupção da operação (botões Parar/Emergência → tela Interrompida), à liberação manual com LED na Home e à tara ao iniciar uma nova dosagem.

A estratégia de testes e os cenários futuros estão documentados em:

→ [`10-testing-strategy.md`](10-testing-strategy.md)

---

## 🔌 Hardware alvo

O dispositivo final deverá utilizar aproximadamente: **ESP32-S3 N16R8**, display 4.3" 800×480, touch capacitivo, **HX711 + load cell**, **SG90 / mecanismo de dispenser**, botões físicos, switch e indicador.

A migração será incremental, substituindo as implementações específicas do simulador pelas implementações físicas.

Detalhes:

→ [`08-target-hardware.md`](08-target-hardware.md)

→ [`11-migration-pc-to-esp32.md`](11-migration-pc-to-esp32.md)

---

## 📚 Documentação completa

A documentação detalhada está nesta pasta (`.docs/`).

| Documento                                                    | Conteúdo                                    |
| ------------------------------------------------------------ | ------------------------------------------- |
| [`00-project-story.md`](00-project-story.md)                 | História, motivação e evolução do projeto   |
| [`01-project-overview.md`](01-project-overview.md)           | Visão geral do sistema                      |
| [`02-architecture.md`](02-architecture.md)                   | Arquitetura e responsabilidades             |
| [`03-architecture-decisions.md`](03-architecture-decisions.md) | Decisões arquiteturais e seus motivos     |
| [`04-domain-and-state-machine.md`](04-domain-and-state-machine.md) | Domínio, estados e regras da dosagem   |
| [`05-hardware-abstraction.md`](05-hardware-abstraction.md)   | Abstrações de hardware                      |
| [`06-simulation-strategy.md`](06-simulation-strategy.md)     | Estratégia de simulação                     |
| [`07-ui-and-navigation.md`](07-ui-and-navigation.md)         | UI e navegação                              |
| [`08-target-hardware.md`](08-target-hardware.md)             | Hardware físico alvo                        |
| [`09-pc-development-environment.md`](09-pc-development-environment.md) | Ambiente de desenvolvimento no PC |
| [`10-testing-strategy.md`](10-testing-strategy.md)           | Estratégia de testes                        |
| [`11-migration-pc-to-esp32.md`](11-migration-pc-to-esp32.md) | Migração do PC para ESP32-S3                |
| [`12-roadmap.md`](12-roadmap.md)                             | Roadmap e evolução do projeto               |
| [`13-agent-guide.md`](13-agent-guide.md)                     | Guia para agentes que modificarem o projeto |

---

## 🤖 Para agentes

Antes de modificar o projeto, leia:

→ [`13-agent-guide.md`](13-agent-guide.md)

Ele contém as regras e o contexto que devem ser considerados antes de alterar arquitetura, domínio, UI, simulação ou hardware.

Regra principal:

> **Não modificar o projeto apenas para fazê-lo funcionar localmente. Preservar sua capacidade de evoluir do simulador para o hardware físico.**

---

## 🧭 Estrutura resumida

```text
.
├── src/
│   ├── main.c
│   ├── hal/
│   ├── ui/
│   ├── domain/
│   └── hardware/
│
├── .docs/
│   ├── 00-project-story.md
│   ├── 01-project-overview.md
│   ├── 02-architecture.md
│   ├── 03-architecture-decisions.md
│   ├── 04-domain-and-state-machine.md
│   ├── 05-hardware-abstraction.md
│   ├── 06-simulation-strategy.md
│   ├── 07-ui-and-navigation.md
│   ├── 08-target-hardware.md
│   ├── 09-pc-development-environment.md
│   ├── 10-testing-strategy.md
│   ├── 11-migration-pc-to-esp32.md
│   ├── 12-roadmap.md
│   └── 13-agent-guide.md
│
├── CMakeLists.txt
├── lv_conf.h
└── README.md
```

---

## Princípio do projeto

```text
Desenvolver no PC
       ↓
Validar o comportamento
       ↓
Isolar hardware
       ↓
Testar cenários
       ↓
Migrar para ESP32-S3
       ↓
Integrar hardware real
       ↓
Calibrar
       ↓
Validar a dosagem física
```

O objetivo não é apenas criar um simulador.

É usar o simulador como base para desenvolver, testar e evoluir o **dosador de ração físico**.

### NOTE:

- [`agent-propose-implementation-plan.md`](agent-propose-implementation-plan.md) contém contexto da proposta inicial do agente + explicação do que foi seguido na proposta inicial e o que "não foi" após termos implementado o que existe atualmente em `src`
- [`implementation.md`](implementation.md) contém um plano inicial do agente do que seria feito
- [`run-code.md`](run-code.md) contém os passos necessários para rodar o projeto
- [`next-steps-clarification.md`](next-steps-clarification.md) explicação do agente sobre os arquivos que ele criou para o `./docs`