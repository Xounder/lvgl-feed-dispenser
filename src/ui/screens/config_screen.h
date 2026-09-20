#ifndef CONFIG_SCREEN_H
#define CONFIG_SCREEN_H

#include "lvgl/lvgl.h"

typedef enum {
    CONFIG_MODE_FIXED_AMOUNT,
    CONFIG_MODE_PORTIONS
} ConfigMode;

lv_obj_t *config_screen_create(ConfigMode mode);

#endif