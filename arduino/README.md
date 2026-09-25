# Sketch Arduino (ESP32-S3)

Pasta de atalho (`junctions`) para rodar o codigo do projeto na placa usando o
**Arduino IDE** como a equipe padrao, sem duplicar arquivos.

## O que ha aqui

- `arduino.ino` — identifica a pasta como sketch (o codigo real esta em `src/`).
- `create_sketch_links.ps1` — recria as junctions `src` e `libraries/lvgl` (nao sao versionadas; rode apos clonar).
- `libraries/lv_conf.h` — Config do LVGL do ESP32 (encaminha para `config/lv_conf_esp32.h`).

Como o `src/` do projeto agora e apenas o codigo compartilhado + plataforma
ESP32 (os arquivos do PC ficam em `src_pc/`), o Arduino compila so o que
interessa ao hardware.

## Como usar

1. Rode `.\arduino\create_sketch_links.ps1` (no PowerShell, na raiz do projeto).
2. Arduino IDE: `File > Open...` e abra `arduino/arduino.ino`.
3. Board Manager: instale **ESP32 by Espressif**; selecione **ESP32S3 Dev Module**.
4. Opcoes da placa (Tools): Flash Size `16MB`, Flash Mode `QIO/OPI`, Partition `Default 16MB`, PSRAM conforme o modulo.
   - Ao selecionar a placa vem com defaults do core, que **nao sao esses**: Flash Size
     costuma vir como `8MB` e Partition como `Default 8MB` — confira e ajuste.
   - Confirme o modulo pelo rotulo no chip: `N16` = flash 16MB, `R8` = PSRAM 8MB.
     Se o modulo for de 8MB de flash, use `8MB` / `Default 8MB`.
   - Flash Mode: QIO (padrao) ou OPI se o modulo tiver flash octal.
   - PSRAM: no S3 Dev Module ja vem como `OPI PSRAM` (sugerido manter ativo).
5. Library Manager: instale as duas bibliotecas abaixo (nova secao "Bibliotecas").
6. Compile e faça upload.

## Bibliotecas (Library Manager)

Busque pelo **nome exato** e confira o **autor** antes de instalar:

| Nome no Manager | Autor | Consumido em |
| --- | --- | --- |
| **ESP32Servo** | Kevin Harrington (madhephaestus), John K. Bennett | `src/hardware/esp32/real_dispenser.cpp` |
| **HX711** | Bogdan Necula (bogde) | `src/hardware/esp32/real_weight_sensor.cpp` |
| **Servo** (classica) | Michael Margolis / Arduino | somente placas AVR (Uno/Mega) — nao usar no ESP32 |

Observacoes:

- Se o Library Manager nao listar nada (indice nao baixa, rede bloqueada),
  instale via `Sketch > Include Library > Add .ZIP Library`:
  - `https://github.com/madhephaestus/ESP32Servo`
  - `https://github.com/bogde/HX711`
- Nao usar a biblioteca **Servo** classica (Michael Margolis / Arduino): ela so
  compila em placas AVR (Uno/Mega) e nao funciona no ESP32.

## Estado

O driver de display/touch (`src/hardware/esp32/board_display.cpp`) ainda e um
stub; a UI so sobe quando ele for implementado (retorna o `lv_display_t`).

## Referencia

Estrutura e decisoes em `.docs/11-migration-pc-to-esp32/` e
`.docs/09-pc-development-environment/`.