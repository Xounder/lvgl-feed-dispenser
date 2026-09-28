
passos:
- Abra o **Developer PowerShell** (Visual Studio), que já traz CMake/MSVC no PATH.

> O prompt mostrado será algo como:
>
> ```bash
> **********************************************************************
> ** Visual Studio 2026 Developer PowerShell v18.10.1
> ** Copyright (c) 2026 Microsoft Corporation
> **********************************************************************
> PS C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools>
> ```

- a partir da pasta clonada do projeto, rode:
  - cmake --build build --config Debug
  - .\bin\Debug\main.exe

> Dica: o projeto não depende de um caminho fixo de clone — rode os comandos a
> partir da raiz do repositório em qualquer máquina (`cd <caminho-do-repositorio>`).