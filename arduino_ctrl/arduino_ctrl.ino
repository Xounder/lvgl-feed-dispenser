/*
 * Sketch do ESP32 EXTERNO na variante de dois ESP32.
 *
 * Variante: ver .docs/14-external-esp32-communication.md.
 * Este sketch roda o dominio (DosingController) + hardware real (HX711, servo,
 * botoes, LED) e conversa por UART com a tela (arduino/). Nao usa LVGL.
 *
 * O papel e fixado em tempo de compilacao por `build_opt.h` (-DROLE_EXTERNAL).
 * Requisitos: rodar `arduino/create_sketch_links.ps1`, instalar HX711 (bogde) e
 * ESP32Servo (madhephaestus) pelo Library Manager e selecionar a placa ESP32S3.
 */
