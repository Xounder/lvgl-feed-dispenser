# Simulador — Dosador de Ração

Simulador desktop **e** firmware ESP32-S3 de um sistema de **dosagem automática
de ração** para animais, escrito em **C** (UI em **LVGL**) com abordagem
*simulation-first*: o comportamento é desenvolvido e validado no PC e depois
portado para o hardware físico (LVGL + SDL2 no PC; LVGL + Espressif no ESP32).

```
UI (LVGL)
    ↓
Domain (DosingController)
    ↓
Hardware Interfaces (WeightSensor, Dispenser)
    ↓
Implementações simuladas (PC) / reais (ESP32)
```

Fluxo da aplicação: **Home → Seleção de modo → Configuração → Dosagem →
(Conclusão | Interrupção)**, com modos de dosagem **Massa (g)** e **Valor (R$)**
e liberação manual.

Documentação completa em [`.docs/README.md`](.docs/README.md) e regras para
agentes em [`AGENTS.md`](AGENTS.md).

---

## Como rodar

### Modo PC (simulador desktop)

Roda `.\bin\Debug\main.exe` (LVGL + SDL2), usando peso e dispenser simulados.

**Pré-requisitos**

- Windows com **Visual Studio Build Tools** (MSVC)
- **CMake**
- **vcpkg** com `SDL2` instalado (x64-windows) — usado pelo toolchain do vcpkg
- LVGL entra como dependência externa (`.gitignore` deixa `lvgl/`, `FreeRTOS/`
  e `vcpkg_installed/` fora do repositório)

**Configurar o build** (uma vez, na raiz do projeto), ajustando os caminhos do
seu ambiente:

```powershell
cmake -S . -B build -G "Visual Studio 18 2026" -A x64 `
  -DCMAKE_TOOLCHAIN_FILE="C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\vcpkg\scripts\buildsystems\vcpkg.cmake" `
  -DCMAKE_PREFIX_PATH="..\vcpkg_installed\x64-windows"
```

**Compilar**

```powershell
cmake --build build --config Debug
```

**Preparar a DLL do SDL2** (Debug): o executável precisa da `SDL2d.dll` ao lado
dele no diretório de execução:

```powershell
Copy-Item .\vcpkg_installed\x64-windows\debug\bin\SDL2d.dll .\bin\Debug\
```

**Executar**

```powershell
.\bin\Debug\main.exe
```

Detalhes do ambiente PC: [`.docs/09-pc-development-environment.md`](.docs/09-pc-development-environment.md)
e [`.docs/run-code.md`](.docs/run-code.md).

---

### Modo Arduino (ESP32-S3)

Compila o mesmo código (UI/domínio/abstrações) com drivers reais de display,
touch, HX711 e dispenser, via **Arduino IDE**.

**Pré-requisitos**

- **Arduino IDE**
- Core **ESP32 by Espressif** (Board Manager)
- Placa-alvo: **ESP32S3 Dev Module** (display RGB 4.3" 800×480 + touch GT911)
- Bibliotecas (Library Manager):

| Nome | Autor | Usada em |
| --- | --- | --- |
| **LVGL 9.6.0** | LVGL (kisvegabor) | toda a UI em `src/` |
| **ESP32Servo** | Kevin Harrington (madhephaestus) | dispenser real |
| **HX711** | Bogdan Necula (bogde) | sensor de peso real |

> Não usar a biblioteca **Servo** clássica (Michael Margolis/Arduino): só
> compila em AVR (Uno/Mega), não serve para ESP32.

**Passos**

1. Recriar as junctions do sketch (não são versionadas):

   ```powershell
   .\arduino\create_sketch_links.ps1
   ```

   > O `lv_conf.h` do sketch encaminha para `config/lv_conf_esp32.h` (fonte
   > única da config LVGL do ESP32). O LVGL **não** entra por junction — vem do
   > Library Manager (9.6.0).

2. Arduino IDE: `File > Open...` e abrir `arduino/arduino.ino`.
3. Selecionar placa **ESP32S3 Dev Module** e ajustar as opções em `Tools` (os defaults do core não são os corretos):
  - USB CDC On Boot: "Disabled"
  - Flash Size: **16MB** (confira o módulo: `N16` = 16MB flash)
  - Flash Mode: **QIO 80MHz**
  - Partition Scheme: **16M Flash (3MB APP/9.9MB FATFS)**
  - PSRAM: conforme o módulo (`R8` = 8MB, sugerido manter **OPI PSRAM** ativo)
  - Serial monitor: **115200** baud (para debug - opcional)
4. Instalar as bibliotecas acima (Library Manager).
5. Compilar e fazer upload.

> Nota: o LVGL 9.6.0 do Library Manager resolve o `lv_conf.h` pelos
> include paths do sketch (`__has_include`), sem tocar na instalação global —
> portátil entre PCs.

Detalhes do sketch: [`arduino/README.md`](arduino/README.md). Migração
PC → ESP32-S3: [`.docs/11-migration-pc-to-esp32.md`](.docs/11-migration-pc-to-esp32.md).

---

## Estrutura

```
├── src/            # compartilhado + plataforma ESP32 (domain/, ui/, hardware/)
├── src_pc/         # somente PC: main.c, hal/ (SDL2), hardware/simulated/
├── arduino/        # sketch Arduino IDE (junctions p/ src/, lv_conf.h, script)
├── config/         # lv_conf_esp32.h (config única do LVGL no ESP32)
├── .docs/          # documentação canônica do projeto
├── CMakeLists.txt  # build do simulador PC
└── lv_conf.h       # config do LVGL no PC
```
