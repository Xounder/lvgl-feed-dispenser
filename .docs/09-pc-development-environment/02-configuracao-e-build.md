# Configuração e build

[voltar ao índice](../09-pc-development-environment.md)

---

## 20. Configuração do CMake

A configuração utilizada atualmente é:

```powershell
cmake -S . -B build -G "Visual Studio 18 2026" -A x64 -DCMAKE_TOOLCHAIN_FILE="C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\vcpkg\scripts\buildsystems\vcpkg.cmake" -DCMAKE_PREFIX_PATH="..\vcpkg_installed\x64-windows"
```

Cada parte possui uma função específica.

---

## 21. `-S .`

Indica que o código-fonte está no diretório atual:

```text
.
```

Ou seja, o `CMakeLists.txt` principal está na raiz do projeto.

---

## 22. `-B build`

Define:

```text
build/
```

como diretório para os arquivos gerados.

Isso mantém os artefatos de compilação separados do código-fonte.

---

## 23. `-G`

A opção:

```text
-G "Visual Studio 18 2026"
```

define o gerador utilizado pelo CMake.

Nesse caso:

```text
Visual Studio 18 2026
```

---

## 24. `-A x64`

Define a arquitetura:

```text
x64
```

Isso é importante porque o SDL2 utilizado foi instalado para:

```text
x64-windows
```

A arquitetura da aplicação e a arquitetura das bibliotecas devem ser compatíveis.

---

## 25. `CMAKE_TOOLCHAIN_FILE`

O parâmetro:

```text
-DCMAKE_TOOLCHAIN_FILE="..."
```

informa ao CMake que deve utilizar o toolchain do vcpkg.

Isso permite que o CMake encontre e configure as dependências instaladas por ele.

---

## 26. `CMAKE_PREFIX_PATH`

O projeto também utiliza:

```text
-DCMAKE_PREFIX_PATH="..\vcpkg_installed\x64-windows"
```

para indicar ao CMake onde encontrar os pacotes instalados.

Isso é especialmente relevante para localizar o SDL2 utilizado pelo simulador.

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

No Windows, a aplicação precisa encontrar a DLL do SDL2 correspondente à configuração utilizada.

Durante o desenvolvimento, a DLL de debug foi copiada para:

```text
bin/Debug/
```

através de:

```powershell
Copy-Item .\vcpkg_installed\x64-windows\debug\bin\SDL2d.dll .\bin\Debug\
```

Isso permite que:

```text
main.exe
```

encontre:

```text
SDL2d.dll
```

no diretório de execução.

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

O CMake possui uma lista de fontes principais semelhante a:

```cmake
set(MAIN_SOURCES
    src/mouse_cursor_icon.c
    src/hal/hal.c
    src/ui/ui.c
    src/ui/screen_manager.c
    src/ui/screens/home_screen.c
    src/ui/screens/mode_screen.c
    src/ui/screens/config_screen.c
    src/ui/screens/dosing_screen.c
    src/ui/screens/completed_screen.c
    src/domain/dosing_controller.c
    src/hardware/simulated/simulated_weight_sensor.c
    src/hardware/simulated/simulated_dispenser.c
)
```

Quando um novo `.c` é adicionado à aplicação, ele precisa ser incluído no sistema de build.

---

## 42. Dependências ignoradas pelo Git

O projeto mantém algumas dependências e artefatos fora do versionamento.

Entre os diretórios ignorados estão:

```text
lvgl/
FreeRTOS/
vcpkg_installed/
build/
bin/
```

O objetivo é evitar versionar:

* bibliotecas externas;
* arquivos gerados;
* binários;
* artefatos temporários.

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