#include "screen_manager.h"
#include "../domain/dosing_controller.h"
#include "screens/home_screen.h"
#include "screens/config_screen.h"
#include "screens/dosing_screen.h"
#include "screens/completed_screen.h"
#include "screens/interrupted_screen.h"

static Screen current_screen;
static DosingMode selected_mode;

static DosingConfig dosing_config = {
    .mode = DOSING_MODE_GRAMS,
    .target_grams = 100,
    .target_money_cents = 500,
    .price_per_kg_cents = 1200
};

void screen_manager_show_config(DosingMode mode)
{
    selected_mode = mode;

    dosing_config.mode = mode;

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

        case SCREEN_INTERRUPTED:
            lv_screen_load(interrupted_screen_create());
            break;
    }
}

DosingConfig *screen_manager_get_dosing_config(void)
{
    return &dosing_config;
}