#ifndef ROLE_SCREEN

#include "physical_controls.h"

#include "../button.h"
#include "../indicator.h"
#include "../../domain/dosing_controller.h"

static int emergency_previous = 0;
static int manual_previous = 0;

void physical_controls_init(void)
{
    emergency_previous = emergency_button.is_pressed();
    manual_previous = manual_release_button.is_pressed();

    indicator.set(0);
}

void physical_controls_poll(void)
{
    int emergency_now = emergency_button.is_pressed();
    int manual_now = manual_release_button.is_pressed();

    if (manual_now && !manual_previous) {
        if (dosing_controller_manual_release_is_allowed()) {
            dosing_controller_manual_release_start();
        }
    } else if (!manual_now && manual_previous) {
        if (dosing_controller_manual_release_is_active()) {
            dosing_controller_manual_release_stop();
        }
    }

    manual_previous = manual_now;

    if (emergency_now && !emergency_previous) {
        if (dosing_controller_get_state() == DOSING_STATE_DOSING) {
            dosing_controller_cancel();
        }
    }

    emergency_previous = emergency_now;

    indicator.set(dosing_controller_manual_release_is_active());
}

#endif
