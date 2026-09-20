# Ambiente de Desenvolvimento no PC

> **Documento canônico** do **ambiente, build e execução** no PC do dosador de ração (simulador desktop LVGL+SDL2 em C; futuro ESP32-S3): Windows + Visual Studio Build Tools 2026, MSVC, CMake 4.3.1-msvc1, vcpkg, SDL2, LVGL, comandos de configuração/build/execução e troubleshooting.
> Este arquivo é o índice; o conteúdo completo está nas partes listadas abaixo.

---

## Resumo

O simulador desktop existe para permitir que a aplicação seja desenvolvida e validada antes da integração com o ESP32-S3 e com o hardware físico.

O ambiente atual utiliza:

```text
Windows
   ↓
Visual Studio Build Tools
   ↓
CMake
   ↓
vcpkg
   ↓
SDL2
   ↓
LVGL
   ↓
Simulador desktop
```

A aplicação é escrita em **C**.

O comando completo de configuração do CMake (com gerador `Visual Studio 18 2026`, arquitetura `x64`, toolchain do vcpkg e `CMAKE_PREFIX_PATH`) está na [parte 02](09-pc-development-environment/02-configuracao-e-build.md).

O [run-code.md](run-code.md) contém apenas o quick-start curto (`cmake --build build --config Debug` e `.\bin\Debug\main.exe`); os detalhes completos de configuração e build estão na [parte 02](09-pc-development-environment/02-configuracao-e-build.md).

O arquivo original excedia 500 linhas e foi dividido em partes lógicas na subpasta `09-pc-development-environment/`, preservando 100% do conteúdo factual.

## Índice

| Parte | Conteúdo |
| ----- | -------- |
| [01-visao-geral-e-toolchain.md](09-pc-development-environment/01-visao-geral-e-toolchain.md) | Objetivo, objetivo do simulador, relação PC/hardware, sistema operacional, Visual Studio Build Tools 2026 (VS 18 2026/x64), compilador, CMake 4.3.1, por que CMake, estrutura do build, diretórios importantes, dependências (LVGL/SDL2), vcpkg, toolchain do vcpkg, SDL2 instalado (2.32.10/x64-windows) e toolchain completo |
| [02-configuracao-e-build.md](09-pc-development-environment/02-configuracao-e-build.md) | Comando completo de configuração do CMake, flags `-S`/`-B`/`-G`/`-A`, `CMAKE_TOOLCHAIN_FILE`, `CMAKE_PREFIX_PATH`, build (`cmake --build build --config Debug`), `SDL2d.dll` ao lado do executável, CMake e fontes (MAIN_SOURCES), dependências ignoradas pelo Git e build versus runtime |
| [03-execucao-e-loop-principal.md](09-pc-development-environment/03-execucao-e-loop-principal.md) | Executável, `main.c`, loop principal, `lv_timer_handler()`, tempo de espera, compatibilidade Windows (`Sleep`), HAL do simulador, `sdl_hal_init(800, 480)`, separação da HAL, estrutura `src/` e `main.c` e domínio |
| [04-configuracao-lvgl.md](09-pc-development-environment/04-configuracao-lvgl.md) | `lv_conf.h`, ThorVG (`LV_USE_THORVG`), avisos do LVGL, demo padrão desabilitado e por que não usar o demo como aplicação |
| [05-simulacao-migracao-e-validacao.md](09-pc-development-environment/05-simulacao-migracao-e-validacao.md) | Ponte com os temas canônicos de simulação, abstração de hardware, migração e testes (pontos que viram link para os documentos donos), PC como ambiente de validação, testes rápidos, dependências não viram código da aplicação, fronteiras e princípios de validação |
| [06-troubleshooting-e-resumo.md](09-pc-development-environment/06-troubleshooting-e-resumo.md) | Problemas comuns (SDL2 não encontrado, DLL ausente, arquitetura errada), reconfiguração do projeto, ambiente mínimo esperado, resumo do ambiente, fluxo de desenvolvimento recomendado e princípio final |

## Documentos relacionados

- [run-code.md](run-code.md) — quick-start curto (build e execução)
- [02-architecture.md](02-architecture.md) — arquitetura, camadas e estrutura `src/`
- [05-hardware-abstraction.md](05-hardware-abstraction.md) — abstrações de hardware (`WeightSensor`/`Dispenser`) e implementações simuladas
- [06-simulation-strategy.md](06-simulation-strategy.md) — estratégia de simulação (peso, taxa, falhas, cenários)
- [07-ui-and-navigation.md](07-ui-and-navigation.md) — UI e navegação (fluxo Home → Mode → Config → Dosing → Completed)
- [10-testing-strategy.md](10-testing-strategy.md) — procedimentos de teste do simulador
- [11-migration-pc-to-esp32.md](11-migration-pc-to-esp32.md) — migração do PC para o ESP32-S3 (GPIO, placa, build ESP-IDF)