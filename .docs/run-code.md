# Executar o simulador (PC)

Na raiz do repositório, o script único cuida de tudo (sincroniza os submodules
`lvgl`/`FreeRTOS`, configura o CMake + vcpkg e compila):

```powershell
.\setup.ps1
```

Ele executa `bin\Debug\main.exe` ao final. Opções:

- `.\setup.ps1 -NoRun` — compila sem executar;
- `.\setup.ps1 -Clean` — apaga `build/` antes de reconfigurar.

Pré-requisito: Windows + Visual Studio Build Tools (MSVC + vcpkg). Na primeira
execução é necessária conexão com a internet (download do SDL2 pelo vcpkg).

Alternativa manual (com `VCPKG_ROOT` definido), a partir de `src_pc`:

```powershell
cmake --preset default
cmake --build --preset debug
```

O executável fica em:

```text
bin\Debug\main.exe
```
