#ifndef CONFIG_SCREEN_H
#define CONFIG_SCREEN_H

#include "lvgl/lvgl.h"
#include "../../domain/dosing_config.h"

lv_obj_t *config_screen_create(DosingMode mode);

#endif