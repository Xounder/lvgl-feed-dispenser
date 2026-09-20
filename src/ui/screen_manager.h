#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "lvgl/lvgl.h"
#include "screens/config_screen.h"
#include "../domain/dosing_config.h"

typedef enum {
    SCREEN_HOME,
    SCREEN_MODE,
    SCREEN_CONFIG,
    SCREEN_DOSING,
    SCREEN_COMPLETED
} Screen;

void screen_manager_init(void);
void screen_manager_show(Screen screen);
void screen_manager_show_config(ConfigMode mode);

DosingConfig *screen_manager_get_dosing_config(void);

#endif