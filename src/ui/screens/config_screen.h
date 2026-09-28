#ifndef CONFIG_SCREEN_H
#define CONFIG_SCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "../../domain/dosing_config.h"

lv_obj_t *config_screen_create(DosingMode mode);

#ifdef __cplusplus
}
#endif

#endif