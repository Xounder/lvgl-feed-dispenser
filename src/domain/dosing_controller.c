#include "dosing_controller.h"
#include "../hardware/weight_sensor.h"
#include "../hardware/dispenser.h"

static DosingConfig *config;
static DosingState state = DOSING_STATE_IDLE;

void dosing_controller_init(DosingConfig *dosing_config)
{
    config = dosing_config;
    state = DOSING_STATE_IDLE;

    simulated_dispenser.stop();
}

void dosing_controller_start(void)
{
    simulated_weight_sensor.reset();

    state = DOSING_STATE_DOSING;

    simulated_dispenser.start();
}

void dosing_controller_update(void)
{
    if (state != DOSING_STATE_DOSING) {
        return;
    }

    int current_weight =
        simulated_weight_sensor.read_grams();

    if (current_weight >= config->target_grams) {
        simulated_dispenser.stop();
        state = DOSING_STATE_COMPLETED;
        return;
    }

    if (simulated_dispenser.is_active()) {
        simulated_weight_sensor.add_grams(2);
    }

    current_weight =
        simulated_weight_sensor.read_grams();

    if (current_weight >= config->target_grams) {
        simulated_dispenser.stop();
        state = DOSING_STATE_COMPLETED;
    }
}

void dosing_controller_cancel(void)
{
    simulated_dispenser.stop();

    state = DOSING_STATE_IDLE;
}

int dosing_controller_get_weight(void)
{
    return simulated_weight_sensor.read_grams();
}

DosingState dosing_controller_get_state(void)
{
    return state;
}