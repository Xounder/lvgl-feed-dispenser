# Configuração e build

[voltar ao índice](../09-pc-development-environment.md)

---

## 20. Configuração do CMake

A configuração é descrita por um **preset** (`src_pc/CMakePresets.json`), que fixa
a arquitetura `x64`, usa o toolchain do vcpkg via `VCPKG_ROOT` e grava o build em
`../build`. O **gerador do Visual Studio não é fixado** — o CMake usa o
instalado na máquina (evita erro de plataforma ao trocar de máquina).

Forma recomendada, um único comando na raiz (descobre vcpkg/CMake, sincroniza os
submodules e compila):

```powershell
.\setup.ps1
```

Manualmente (com `VCPKG_ROOT` definido), a partir de `src_pc`:

```powershell
cmake --preset default
cmake --build --preset debug
```

Cada parte possui uma função específica (abaixo).

---

## 21. Fonte (`-S src_pc`)

O `CMakeLists.txt` principal do simulador PC fica em `src_pc/`, na raiz do
repositório:

```text
src_pc/
```

O preset já aponta essa pasta como fonte (`sourceDir`), o que equivale ao
`-S src_pc` do modo manual.

---

## 22. Diretório de build (`-B build`)

Os arquivos gerados vão para:

```text
build/
```

Isso mantém os artefatos de compilação separados do código-fonte. O preset
define esse diretório (`binaryDir`), equivalente ao `-B build` do modo manual.

---

## 23. Gerador

O preset **não** define `-G`: o CMake usa o gerador do Visual Studio instalado na
máquina. Fixar uma versão (por exemplo, `Visual Studio 18 2026`) quebraria em
qualquer máquina com outra versão instalada.

---

## 24. Arquitetura `x64`

O preset define a arquitetura:

```json
"architecture": { "value": "x64", "strategy": "set" }
```

Isso é importante porque o SDL2 é instalado para:

```text
x64-windows
```

A arquitetura da aplicação e a arquitetura das bibliotecas devem ser compatíveis.

---

## 25. `CMAKE_TOOLCHAIN_FILE`

O preset aponta o toolchain do vcpkg pela variável de ambiente `VCPKG_ROOT`:

```json
"toolchainFile": "$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
```

Isso evita caminho fixo: o `setup.ps1` descobre o vcpkg (via `VCPKG_ROOT` ou a
instalação do Build Tools) e exporta a variável antes de chamar o CMake.

---

## 26. Localização do `vcpkg_installed`

O `vcpkg_installed` (pacotes instalados, incluindo o SDL2) fica **dentro de
`build/`** (que é ignorado pelo Git), em modo *manifest* (`src_pc/vcpkg.json`):

```text
build/vcpkg_installed/x64-windows
```

O próprio toolchain do vcpkg aponta esse diretório ao CMake, então o comando de
configuração não precisa de um `-DCMAKE_PREFIX_PATH` explícito.

---

## 27. Build

Depois da configuração, o projeto pode ser compilado através do CMake.

O resultado atual é:

```text
bin/Debug/main.exe
```

A configuração utilizada é:

```text
Debug
```

---

## 28. DLL do SDL2

No Windows, a aplicação precisa encontrar a DLL do SDL2 correspondente à
configuração utilizada.

O vcpkg copia a DLL automaticamente para junto do executável
(`VCPKG_APPLOCAL_DEPS`), não sendo preciso copiar à mão:

```text
bin/Debug/
├── main.exe
└── SDL2d.dll
```

Isso permite que `main.exe` encontre `SDL2d.dll` no diretório de execução.

---

## 29. Por que existe `SDL2d.dll`

A letra:

```text
d
```

indica a variante de debug utilizada pelo SDL2 no ambiente atual.

A aplicação compilada em Debug deve utilizar a biblioteca compatível com essa configuração.

A escolha da DLL deve acompanhar a configuração do build.

---

## 41. CMake e fontes

O CMake possui uma lista de fontes principais semelhante a (no `src_pc/CMakeLists.txt`):

```cmake
set(MAIN_SOURCES
    mouse_cursor_icon.c
    hal/hal.c
    ../src/ui/ui.c
    ../src/ui/screen_manager.c
    ../src/ui/screens/screen_chrome.c
    ../src/ui/screens/home_screen.c
    ../src/ui/screens/config_screen.c
    ../src/ui/screens/dosing_screen.c
    ../src/ui/screens/completed_screen.c
    ../src/ui/screens/interrupted_screen.c
    ../src/ui/screens/manual_release_widget.c
    ../src/domain/dosing_controller.c
    hardware/simulated/simulated_weight_sensor.c
    hardware/simulated/simulated_dispenser.c
)
```

Quando um novo `.c` é adicionado à aplicação, ele precisa ser incluído no sistema de build.

---

## 42. Dependências e artefatos fora do Git

O projeto mantém dependências externas e artefatos fora do versionamento:

```text
build/
bin/
vcpkg_installed/
```

O objetivo é evitar versionar arquivos gerados, binários e artefatos temporários.

O **LVGL** e o **FreeRTOS** já não ficam em `.gitignore`: são **submodules**,
clonados com `git submodule update --init --recursive` (o `setup.ps1` faz isso
automaticamente).

---

## 55. Build versus runtime

É importante separar duas etapas.

### Configuração/build

```text
CMake
 ↓
vcpkg
 ↓
MSVC
 ↓
main.exe
```

### Execução

```text
main.exe
 ↓
LVGL
 ↓
SDL2
 ↓
janela
```

O CMake e o vcpkg são ferramentas de desenvolvimento/build.

Eles não fazem parte do runtime do dispositivo ESP32-S3.