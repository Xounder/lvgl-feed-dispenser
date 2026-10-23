# AGENTS.md

Instruções para agentes de IA que trabalham neste repositório.

## Propósito do projeto

Simulador PC (LVGL + SDL2, em C) de um **dosador de ração** para animais. O
objetivo é desenvolver e validar a UI/domínio no PC e depois portar o mesmo
código para o alvo embarcado ESP32-S3 (ver `.docs/11-migration-pc-to-esp32/`).

Referências canônicas de requisitos:

- `.docs/Trabalho.md` — requisitos acadêmicos (tabelas **RP** = requisitos de
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
  apenas os textos (`telas-regras.md`, `Trabalho.md`, docs, em `.docs/`).

Build PC — o script da raiz faz tudo (sync de submodules + configure/build/run).
Para o usuário rodar:
```powershell
.\setup.ps1            # compila e executa
.\setup.ps1 -NoRun     # apenas compila
.\setup.ps1 -Clean     # apaga build/ antes de reconfigurar
```

Alternativa sem o script (requer `VCPKG_ROOT` definido), a partir de `src_pc`:
```powershell
cmake --preset default
cmake --build --preset debug
```

Alternativa quando o agente precisa checar compilação/link (MSBuild localizado
via `vswhere`, sem caminho fixo):
```powershell
$msbuild = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -find "MSBuild\**\Bin\MSBuild.exe"
& $msbuild build\main.vcxproj -p:Configuration=Debug -v:m
```
> **Atenção:** quando um **novo arquivo** `.c` é adicionado ao
> `src_pc/CMakeLists.txt` (MAIN_SOURCES), o build incremental do MSBuild pode não
> compilá-lo (LNK2019 em `main.exe`). Nesse caso force rebuild completo:
> `-t:Rebuild`.

Após qualquer mudança, peça ao usuário para recompilar/rodar e validar o
comportamento na GUI.

Build ESP32 (usuário, Arduino IDE + core ESP32):
```powershell
powershell -ExecutionPolicy Bypass -File .\arduino\create_sketch_links.ps1
# abrir arduino/arduino.ino na Arduino IDE, placa ESP32S3 Dev Module,
# flash 16MB QIO/OPI, Partition "Default 16MB", PSRAM conforme o módulo.
# Libs do Library Manager: LVGL 9.6.0 (kisvegabor), HX711 (bogde),
# ESP32Servo (madhephaestus).
```
> O LVGL vem do **Library Manager** (global), não de junction `arduino/libraries/lvgl`.
> O `lv_conf.h` da raiz do sketch (`arduino/lv_conf.h`) encaminha para
> `config/lv_conf_esp32.h` via include path do Arduino (`__has_include`).

## Estrutura do código

- `src/` — código compartilhado + plataforma ESP32: `domain/`, `ui/`,
  `hardware/` (interfaces + `esp32/` real) e `main_esp32.cpp` (entry setup/loop).
- `src_pc/` — **somente PC**: `main.c`, `freertos_main.c`, `mouse_cursor_icon.c`,
  `hal/` (SDL2) e `hardware/simulated/`.
- `arduino/` — sketch Arduino IDE com junction p/ `src/` (recriar com
  `create_sketch_links.ps1`); `lv_conf.h` (raiz) aponta p/ `config/lv_conf_esp32.h`;
  LVGL 9.6.0 vem do Library Manager global.
- `src/domain/dosing_controller.{c,h}` — lógica de domínio pura (sem LVGL):
  estado, dosagem em 2 etapas, liberação manual, eventos, inicialização.
- `src/ui/screen_manager.h` — enum de telas e função `screen_manager_show()`.
- `src/ui/screens/` — uma tela por arquivo, criada por `xxx_screen_create()`:
  `home_screen.c`, `mode_screen.c`, `config_screen.c`, `dosing_screen.c`,
  `completed_screen.c`, `interrupted_screen.c`, `manual_release_screen.c`.
- `src/ui/components/` — widgets reutilizáveis: `screen_chrome.{c,h}`,
  `manual_release_widget.{c,h}`, `ui_button.{c,h}`, `ui_metric.{c,h}`,
  `ui_meta_row.{c,h}`, `ui_progress.{c,h}`.
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
- O `README.md` dentro do submodule `lvgl/` é o stock do LVGL — **não alterar** (o `README.md` da raiz é do projeto).
- Toda mudança de comportamento deve ser refletida nos docs afetados.

## Convenções de código

- **Sem comentários no código**, salvo quando solicitado.
- Seguir os padrões já existentes nas telas (Layout/estilo LVGL presentes).
- Não adicionar dependências novas sem necessidade; aproveitar componentes
  existentes (ex.: `manual_release_widget` para liberação manual).

## Git

- **Não commitar** a menos que o usuário peça explicitamente.