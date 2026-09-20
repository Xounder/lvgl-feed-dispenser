# Troubleshooting, ambiente mínimo e princípios

[voltar ao índice](../09-pc-development-environment.md)

---

## 60. Problemas comuns

### SDL2 não encontrado

Verificar:

```text
vcpkg
CMAKE_TOOLCHAIN_FILE
CMAKE_PREFIX_PATH
arquitetura x64
```

---

### Executável inicia e DLL está ausente

Verificar se:

```text
SDL2d.dll
```

está disponível no diretório esperado para a configuração Debug.

---

### Build usa arquitetura errada

Verificar:

```text
-A x64
```

e se a dependência também é:

```text
x64-windows
```

---

### CMake não encontra o pacote

Reconfigurar usando o toolchain correto do vcpkg.

---

## 61. Reconfiguração do projeto

Quando alterações importantes forem feitas no ambiente de build, pode ser necessário reconfigurar:

```text
build/
```

O princípio é:

```text
CMakeLists.txt
   ↓
configure
   ↓
build/
```

Se o estado gerado estiver inconsistente, uma reconstrução limpa pode ser necessária.

Não é necessário apagar o código-fonte nem as dependências apenas por causa de um problema no diretório `build`.

---

## 67. Ambiente mínimo esperado

Para reproduzir o ambiente de desenvolvimento atual, deve existir:

```text
Windows
Visual Studio Build Tools
MSVC
CMake
vcpkg
SDL2
LVGL
```

Além do código-fonte do projeto.

---

## 68. Resumo do ambiente

| Componente                | Função                              |
| ------------------------- | ----------------------------------- |
| Windows                   | plataforma atual de desenvolvimento |
| Visual Studio Build Tools | toolchain MSVC                      |
| CMake                     | configuração e build                |
| vcpkg                     | gerenciamento de dependências       |
| SDL2                      | plataforma gráfica do simulador     |
| LVGL                      | framework gráfico                   |
| C                         | linguagem da aplicação              |
| `build/`                  | artefatos do CMake                  |
| `bin/Debug/`              | executável e DLLs necessárias       |

---

## 69. Fluxo de desenvolvimento recomendado

O ciclo normal é:

```text
Editar código
    ↓
CMake/build
    ↓
Executar main.exe
    ↓
Testar UI
    ↓
Testar comportamento
    ↓
Corrigir
    ↓
Rebuild
    ↓
Repetir
```

Quando o comportamento estiver validado:

```text
PC
 ↓
implementação real
 ↓
ESP32-S3
```

A execução rápida usa o quick-start de [run-code.md](../run-code.md).

---

## 70. Princípio final

O ambiente PC deve ser tratado como uma **plataforma de desenvolvimento e validação**, e não como uma versão descartável do projeto.

Sua função é permitir que a maior parte possível do comportamento seja desenvolvida antes da integração física.

A arquitetura desejada é:

```text
                  PROJETO
                     │
          ┌──────────┴──────────┐
          │                     │
          ▼                     ▼
       SIMULADOR              ESP32-S3
          │                     │
       SDL2/HAL             HAL/Drivers
          │                     │
          └──────────┬──────────┘
                     │
                     ▼
              Aplicação comum
                     │
          ┌──────────┴──────────┐
          ▼                     ▼
         UI                  Domain
          │                     │
          └──────────┬──────────┘
                     ▼
             Hardware abstraction
```

O objetivo do ambiente não é simplesmente "rodar LVGL no Windows".

É permitir o desenvolvimento incremental do produto:

> **validar no PC o que pode ser validado no PC, manter as dependências específicas da plataforma isoladas e levar para o ESP32-S3 apenas o que realmente precisa ser físico.**

Dessa forma, CMake, Visual Studio Build Tools, vcpkg e SDL2 formam a infraestrutura do simulador, enquanto LVGL, a UI, o domínio e as abstrações de hardware permanecem alinhados com a arquitetura que posteriormente será executada no dispositivo físico.