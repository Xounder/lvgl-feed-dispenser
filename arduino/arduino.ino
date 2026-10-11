/*
 * Sketch da TELA (ESP32-S3 touch).
 *
 * Variante com ESP32 externo: ver .docs/14-external-esp32-communication.md.
 * Este sketch compila apenas a UI (LVGL) + o proxy UART. O controle fisico
 * (HX711, servo, botoes, LED) roda no ESP32 externo (arduino_ctrl/).
 *
 * O papel e fixado em tempo de compilacao por `build_opt.h` (-DROLE_SCREEN):
 * o Arduino IDE compila toda a arvore do sketch (junction src/), logo nao ha
 * como escolher tela/externo em runtime.
 *
 * Requisitos antes de abrir/compilar:
 *  - Rodar `create_sketch_links.ps1` (recria a junction `arduino/src`).
 *  - Instalar pelo Library Manager: "LVGL" 9.6.0 (unico necessario aqui).
 *  - Placa: ESP32S3 Dev Module; Flash Size 16MB; Flash Mode QIO/OPI;
 *    Partition Scheme "Default 16MB"; PSRAM conforme o modulo (OPI se tiver).
 *
 * A config do LVGL para o ESP32 e resolvida pelo `lv_conf.h` desta pasta, que
 * encaminha para `config/lv_conf_esp32.h`.
 */
