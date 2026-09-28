# Visão geral e toolchain

[voltar ao índice](../09-pc-development-environment.md)

---

## 1. Objetivo

Este documento descreve o ambiente utilizado para desenvolver e executar o simulador desktop do projeto.

O simulador existe para permitir que a aplicação seja desenvolvida e validada antes da integração com o ESP32-S3 e com o hardware físico.

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

A aplicação é escrita em **C**, em migração para **C++** (decisão em [03-architecture-decisions.md](../03-architecture-decisions.md)).

---

## 2. Objetivo do simulador

O simulador PC não é apenas uma forma de visualizar a interface.

Ele existe para permitir validar:

* estrutura da aplicação;
* navegação;
* comportamento da UI;
* configuração da dosagem;
* máquina de estados;
* controller;
* abstrações de hardware;
* comportamento simulado do peso;
* comportamento simulado do dispenser;
* fluxo completo de dosagem.

A ideia é reduzir a quantidade de problemas que só seriam descobertos depois que o ESP32-S3 estivesse disponível.

---

## 3. Relação entre PC e hardware final

O PC não é o produto final.

Ele é o ambiente de desenvolvimento e validação.

A relação conceitual entre a aplicação comum, o simulador (SDL2/LVGL + hardware simulado) e o hardware (ESP32-S3/LVGL + hardware real) é apresentada em [02-architecture.md](../02-architecture.md) e, para o ESP32, em [11-migration-pc-to-esp32.md](../11-migration-pc-to-esp32.md).

---

## 4. Sistema operacional

O desenvolvimento atual ocorre no:

```text
Windows
```

O projeto utiliza ferramentas do ecossistema Microsoft para compilação C/C++.

A principal ferramenta de compilação instalada é:

```text
Visual Studio Build Tools
```

Não é necessário utilizar o Visual Studio IDE completo para construir o projeto.

---

## 5. Visual Studio Build Tools

A versão utilizada durante a configuração do projeto é:

```text
Visual Studio Build Tools 2026
```

Ela fornece o ambiente necessário para compilação utilizando o compilador da Microsoft.

O ambiente inclui o toolchain utilizado pelo CMake para gerar a solução Visual Studio.

A geração atual utiliza:

```text
Visual Studio 18 2026
```

com arquitetura:

```text
x64
```

---

## 6. Compilador

O projeto é compilado utilizando o toolchain do Visual Studio.

O CMake gera um projeto:

```text
Visual Studio 18 2026
```

para:

```text
x64
```

A vantagem dessa abordagem é utilizar um ambiente de compilação integrado ao Windows e compatível com as bibliotecas utilizadas pelo simulador.

---

## 7. CMake

O sistema de build utilizado é:

```text
CMake
```

A versão utilizada durante a configuração atual é:

```text
CMake 4.3.1-msvc1
```

O CMake é responsável por:

* definir o projeto;
* registrar os arquivos `.c`;
* configurar includes;
* localizar dependências;
* configurar o compilador;
* gerar o projeto do Visual Studio;
* definir diretórios de build;
* integrar o vcpkg.

---

## 8. Por que CMake

O projeto poderia utilizar diretamente arquivos de solução do Visual Studio.

Porém, o CMake mantém a definição do projeto mais independente da IDE.

Em vez de depender diretamente de:

```text
.vcxproj
.sln
```

o projeto possui uma definição:

```text
src_pc/CMakeLists.txt
```

Isso facilita uma futura compilação em outro ambiente.

---

## 9. Estrutura básica do build

O fluxo utilizado é:

```text
src_pc/CMakeLists.txt
      ↓
cmake configure
      ↓
build/
      ↓
Visual Studio project
      ↓
compilação
      ↓
bin/Debug/main.exe
```

A pasta `build/` contém artefatos gerados pelo CMake e não representa o código-fonte da aplicação.

---

## 10. Diretórios importantes

A estrutura da raiz do projeto (`src/`, `src_pc/`, `.docs/`, `lvgl/`, `FreeRTOS/`, `build/`, `bin/`) está em [02-architecture.md](../02-architecture.md). O `vcpkg_installed/` fica fora do repositório, na pasta irmã do projeto.

Algumas dessas pastas são dependências ou artefatos gerados e não fazem parte do código principal versionado.

As dependências ignoradas pelo Git são detalhadas na seção 42 da parte [02-configuracao-e-build.md](./02-configuracao-e-build.md).

---

## 11. Dependências externas

O projeto utiliza principalmente:

```text
LVGL
SDL2
```

O LVGL fornece a biblioteca gráfica.

O SDL2 fornece o ambiente necessário para executar a interface gráfica no PC.

---

## 12. LVGL

O projeto utiliza:

```text
LVGL
```

como biblioteca gráfica.

O LVGL é responsável por:

* objetos gráficos;
* telas;
* botões;
* labels;
* barras;
* eventos;
* timers;
* gerenciamento da interface;
* abstração do input/output gráfico.

A aplicação utiliza LVGL tanto conceitualmente quanto diretamente em seus módulos de UI.

---

## 13. LVGL no simulador

No hardware:

```text
LVGL
   ↓
driver de display
   ↓
display físico
```

No PC:

```text
LVGL
   ↓
SDL2 HAL
   ↓
janela do sistema
```

Isso permite executar a mesma biblioteca gráfica sem precisar do ESP32-S3.

---

## 14. SDL2

O SDL2 é utilizado como camada de execução gráfica no PC.

Ele fornece recursos necessários para:

* criação da janela;
* renderização;
* eventos de entrada;
* mouse;
* teclado;
* integração com a aplicação gráfica.

No contexto deste projeto, o SDL2 funciona como uma plataforma de simulação para o LVGL.

---

## 15. vcpkg

O gerenciamento da dependência SDL2 é realizado com:

```text
vcpkg
```

A versão utilizada vem integrada ao ambiente do Visual Studio Build Tools.

O executável utilizado atualmente está em:

```text
C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\vcpkg\vcpkg.exe
```

---

## 16. Toolchain do vcpkg

O arquivo de integração utilizado pelo CMake é:

```text
C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\vcpkg\scripts\buildsystems\vcpkg.cmake
```

Ele é passado ao CMake através de:

```text
-DCMAKE_TOOLCHAIN_FILE="..."
```

---

## 17. SDL2 instalado pelo vcpkg

A versão utilizada atualmente é:

```text
SDL2 2.32.10
```

para:

```text
x64-windows
```

O pacote instalado é:

```text
sdl2:x64-windows@2.32.10#1
```

A instalação local está em (fora do repositório, na pasta irmã do projeto):

```text
../vcpkg_installed/x64-windows
```

---

## 18. Por que SDL2 é uma dependência de PC

O SDL2 existe para permitir a execução desktop.

Ele não representa o hardware final.

Portanto:

```text
SDL2
```

é uma dependência da plataforma:

```text
PC simulator
```

e não uma dependência que deve ser levada para o ESP32-S3.

A separação é importante:

```text
                 Código da aplicação
                        │
                ┌───────┴───────┐
                │               │
              PC              ESP32
                │               │
              SDL2          drivers reais
```

---

## 19. Toolchain completo

O ambiente atual pode ser visualizado assim:

```text
┌─────────────────────────────┐
│          Windows            │
├─────────────────────────────┤
│ Visual Studio Build Tools   │
│                             │
│ MSVC                        │
│ CMake                       │
│ vcpkg                       │
└──────────────┬──────────────┘
               │
               ▼
          CMake Project
               │
       ┌───────┴────────┐
       │                │
      LVGL             SDL2
       │                │
       └───────┬────────┘
               ▼
        Simulator.exe
```