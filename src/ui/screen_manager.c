#include "screen_manager.h"
#include "screens/home_screen.h"
#include "screens/mode_screen.h"
#include "screens/config_screen.h"
#include "screens/dosing_screen.h"
#include "screens/completed_screen.h"

static Screen current_screen;
static ConfigMode selected_mode;

static DosingConfig dosing_config = {
    .target_grams = 100,
    .portions = 1
};

void screen_manager_show_config(ConfigMode mode)
{
    selected_mode = mode;

    lv_screen_load(config_screen_create(selected_mode));
}

void screen_manager_init(void)
{
    current_screen = SCREEN_HOME;

    dosing_controller_init(&dosing_config);

    screen_manager_show(SCREEN_HOME);
}

void screen_manager_show(Screen screen)
{
    current_screen = screen;

    switch (screen) {
        case SCREEN_HOME:
            lv_screen_load(home_screen_create());
            break;

        case SCREEN_MODE:
            lv_screen_load(mode_screen_create());
            break;

        case SCREEN_CONFIG:
            lv_screen_load(config_screen_create(selected_mode));
            break;

        case SCREEN_DOSING:
            dosing_controller_start();
            lv_screen_load(dosing_screen_create());
            break;

        case SCREEN_COMPLETED:
            lv_screen_load(completed_screen_create());
            break;
    }
}

DosingConfig *screen_manager_get_dosing_config(void)
{
    return &dosing_config;
}