/*
 * Projeto: simulador/porta do dosador de racao para ESP32-S3.
 *
 * Este sketch so identifica a pasta `arduino/` como um projeto Arduino IDE.
 * O codigo real vive em `src/` (mesma arvore do projeto) e o setup()/loop()
 * estao em `src/main_esp32.cpp`, que e compilado automaticamente junto com
 * o resto de `src/` pelo Arduino.
 *
 * Requisitos antes de abrir/compilar:
 *  - Rodar `create_sketch_links.ps1` (recria as junctions `arduino/src` e
 *    `arduino/libraries/lvgl` para apontar para o repositorio).
 *  - Instalar pelo Library Manager: LVGL NAO e instalado (usa a junction),
 *    instalar "HX711" (bogde) e "ESP32Servo" (madhephaestus).
 *  - Placa: ESP32S3 Dev Module; Flash Size 16MB; Flash Mode QIO/OPI;
 *    Partition Scheme "Default 16MB"; PSRAM conforme o modulo (OPI se tiver).
 *  - Serial: 115200 baud (monitor) com USB CDC habilitado, se aplicavel.
 *
 * A config do LVGL para o ESP32 e resolvida pela library em
 * `libraries/lv_conf.h`, que encaminha para `config/lv_conf_esp32.h`.
 *
 * Nota: o driver de display/touch (src/hardware/esp32/board_display.cpp) ainda
 * e um stub - so compila a UI quando implementado (retorna o lv_display_t).
 */