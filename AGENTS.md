# AGENTS.md

Instruções para agentes de IA que trabalham neste repositório.

## Propósito do projeto

Simulador PC (LVGL + SDL2, em C) de um **dosador de ração** para animais. O
objetivo é desenvolver e validar a UI/domínio no PC e depois portar o mesmo
código para o alvo embarcado ESP32-S3 (ver `.docs/11-migration-pc-to-esp32/`).

Referências canônicas de requisitos:

- `Trabalho.md` — requisitos acadêmicos (tabelas **RP** = requisitos de
  produto, **RS** = requisitos de software) e regras de telas.
- `.images/telas-regras.md` — regras canônicas de fluxo/layout de telas,
  acompanhadas dos mockups `.images/*.png`.
- `.docs/01-project-overview.md` — visão geral, incluindo a **tabela de
  rastreabilidade RP/RS completa** (~linha 273).

## Ambiente e execução (IMPORTANTE)

**O agente não consegue executar nem validar visualmente o projeto:**

- `cmake`, `cl`, `gcc` **não estão no PATH** do ambiente do agente.
- A GUI do simulador (`main.exe`) exige display e interação; quem valida é o
  **usuário**.
- O agente **não consegue ver imagens PNG** (mockups em `.images/`) — usar
  apenas os textos (`telas-regras.md`, `Trabalho.md`, docs).

Build manual (pelo usuário, VS Build Tools já instalado):
```powershell
cmake -S . -B build
cmake --build build --config Debug
& .\bin\Debug\main.exe
```
Alternativa quando o agente precisa checar compilação/link:
```powershell
& "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\MSBuild\Current\Bin\MSBuild.exe" build\main.vcxproj -p:Configuration=Debug -v:m
```
> **Atenção:** quando um **novo arquivo** `.c` é adicionado ao
> `CMakeLists.txt` (MAIN_SOURCES), o build incremental do MSBuild pode não
> compilá-lo (LNK2019 em `main.exe`). Nesse caso force rebuild completo:
> `-t:Rebuild`.

Após qualquer mudança, peça ao usuário para recompilar/rodar e validar o
comportamento na GUI.

## Estrutura do código

- `src/domain/dosing_controller.{c,h}` — lógica de domínio pura (sem LVGL):
  estado, dosagem em 2 etapas, liberação manual, eventos, inicialização.
- `src/ui/screen_manager.h` — enum de telas e função `screen_manager_show()`.
- `src/ui/screens/` — uma tela por arquivo, criada por `xxx_screen_create()`:
  `home_screen.c`, `mode_screen.c`, `config_screen.c`, `dosing_screen.c`,
  `completed_screen.c`, `interrupted_screen.c`. Também o widget
  `manual_release_widget.{c,h}` (LED + botão, reutilizado por 3 telas).
- `lvgl/`, `FreeRTOS/`, `build/`, `bin/` — dependências/submódulos e artefatos.

## Modelo de domínio (resumo)

- Modos: **Massa | Valor (R$)** (`DosingConfig`: `target_grams`,
  `target_money_cents`, `price_per_kg_cents`).
- Estados: `IDLE` (Aguardando), `DOSING` (Dosando), `COMPLETED` (Concluído),
  `INTERRUPTED` (Interrompido); fase `FAST | FINE` (rápida +20 g, precisa +2 g
  quando faltam ≤30 g).
- **Liberação manual** (RS14/RS15): disponível em `IDLE`, `COMPLETED` e
  `INTERRUPTED`; **bloqueada durante `DOSING`** (RS15), incluindo o botão
  físico (RS16) refletido no **LED**.
- Conversão: `grams = (cents * 1000) / price_per_kg_cents`.

## Documentação (`.docs/`) — regras

- Editar **direto em `.docs/`**; **nunca** criar arquivos na pasta `audited/`.
- **Cada `.md` deve ter ≤500 linhas.**
- **Links relativos sem prefixo `docs/`**; não criar novos arquivos de docs.
- Manter índices/estrutura de cada documento.
- **Idioma: PT-BR** (respostas e conteúdo dos docs).
- Diretórios canônicos: `02-architecture/`, `03-architecture-decisions/`,
  `04-domain-and-state-machine/`, `05-hardware-abstraction/`,
  `06-simulation-strategy/`, `07-ui-and-navigation/`, `08-target-hardware/`,
  `09-pc-development-environment/`, `10-testing-strategy/`,
  `11-migration-pc-to-esp32/`, `12-roadmap/`, `13-agent-guide/`.
- Arquivos `agent-propose-implementation-plan/`, `implementation/` e os `.md`
  rasos antigos (ex.: `02-architecture.md`) são **históricos — não atualizar**.
- `README.md` (raiz) é o stock do LVGL — **não alterar**.
- Toda mudança de comportamento deve ser refletida nos docs afetados.

## Convenções de código

- **Sem comentários no código**, salvo quando solicitado.
- Seguir os padrões já existentes nas telas (Layout/estilo LVGL presentes).
- Não adicionar dependências novas sem necessidade; aproveitar componentes
  existentes (ex.: `manual_release_widget` para liberação manual).

## Git

- **Não commitar** a menos que o usuário peça explicitamente.