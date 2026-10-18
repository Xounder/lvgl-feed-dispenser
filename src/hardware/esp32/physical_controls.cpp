#include "physical_controls.h"

#include <Arduino.h>

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

    Serial.printf(
        "[CTRL] init emergencia=%d liberacao=%d\n",
        emergency_previous,
        manual_previous
    );
}

void physical_controls_poll(void)
{
    int emergency_now = emergency_button.is_pressed();
    int manual_now = manual_release_button.is_pressed();

    if (manual_now && !manual_previous) {
        if (dosing_controller_manual_release_is_allowed()) {
            dosing_controller_manual_release_start();
            Serial.println("[CTRL] liberacao manual INICIADA");
        } else {
            Serial.println("[CTRL] liberacao manual BLOQUEADA (dosando)");
        }
    } else if (!manual_now && manual_previous) {
        if (dosing_controller_manual_release_is_active()) {
            dosing_controller_manual_release_stop();
            Serial.println("[CTRL] liberacao manual PARADA");
        }
    }

    manual_previous = manual_now;

    if (emergency_now && !emergency_previous) {
        if (dosing_controller_get_state() == DOSING_STATE_DOSING) {
            dosing_controller_cancel();
            Serial.println("[CTRL] EMERGENCIA: dosagem cancelada");
        } else {
            Serial.println("[CTRL] EMERGENCIA ignorada (sem dosagem ativa)");
        }
    }

    emergency_previous = emergency_now;

    indicator.set(dosing_controller_manual_release_is_active());
}
