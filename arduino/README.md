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
4. Opcoes da placa: Flash Size `16MB`, Flash Mode `QIO/OPI`, Partition `Default 16MB`, PSRAM conforme o modulo.
5. Library Manager: instale **HX711** (Bogdan Necula/bogde) e **ESP32Servo** (Kevin Harrington/madhephaestus).
6. Compile e faça upload.

## Estado

O driver de display/touch (`src/hardware/esp32/board_display.cpp`) ainda e um
stub; a UI so sobe quando ele for implementado (retorna o `lv_display_t`).

## Referencia

Estrutura e decisoes em `.docs/11-migration-pc-to-esp32/` e
`.docs/09-pc-development-environment/`.