/*
 * lv_conf.h do sketch Arduino.
 *
 * O LVGL 9.6.0 (Library Manager global) resolve o lv_conf.h pelos include
 * paths do sketch (mecanismo __has_include/LV_CONF_INCLUDE_SIMPLE do
 * lv_conf_internal.h). Este arquivo apenas encaminha para a config unica do
 * ESP32, mantendo uma unica fonte de verdade: `config/lv_conf_esp32.h`.
 */
#include "../config/lv_conf_esp32.h"