/*
 * lv_conf.h do sketch Arduino.
 *
 * O LVGL (library em `libraries/lvgl`, via junction) procura o lv_conf.h
 * ao lado da pasta lvgl (mechanismo padrao do LVGL: `../../../../lv_conf.h`
 * a partir de lvgl/include/lvgl/config/lv_conf_internal.h).
 *
 * Este arquivo apenas encaminha para a config unica do ESP32, mantendo uma
 * unica fonte de verdade: `config/lv_conf_esp32.h`.
 */
#include "../../config/lv_conf_esp32.h"