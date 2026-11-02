#include "dosing_controller.h"
#include "../hardware/weight_sensor.h"
#include "../hardware/dispenser.h"

#define FAST_STEP_GRAMS 20
#define FINE_STEP_GRAMS 2
#define FINE_THRESHOLD_GRAMS 30
#define MANUAL_STEP_GRAMS 5
#define MAX_VALID_GRAMS 40000

static DosingConfig *config;
static DosingState state = DOSING_STATE_IDLE;
static DosingPhase phase = DOSING_PHASE_FAST;
static int manual_release_active = 0;

static int effective_target_grams(void)
{
    if (config->mode == DOSING_MODE_GRAMS) {
        return config->target_grams;
    }

    if (config->price_per_kg_cents <= 0) {
        return 0;
    }

    return (config->target_money_cents * 1000) / config->price_per_kg_cents;
}

void dosing_controller_init(DosingConfig *dosing_config)
{
    config = dosing_config;
    state = DOSING_STATE_IDLE;
    phase = DOSING_PHASE_FAST;
    manual_release_active = 0;

    dispenser.stop();
}

void dosing_controller_start(void)
{
    if (state == DOSING_STATE_DOSING) {
        return;
    }

    manual_release_active = 0;

    int target = effective_target_grams();
    int current_weight = weight_sensor.read_grams();

    if (target <= 0 || target > MAX_VALID_GRAMS ||
        current_weight < 0 || current_weight > MAX_VALID_GRAMS) {
        dispenser.stop();
        state = DOSING_STATE_INTERRUPTED;
        return;
    }

    if (current_weight >= target) {
        dispenser.stop();
        state = DOSING_STATE_COMPLETED;
        return;
    }

    phase = target - current_weight <= FINE_THRESHOLD_GRAMS ?
        DOSING_PHASE_FINE : DOSING_PHASE_FAST;
    state = DOSING_STATE_DOSING;

    dispenser.start(phase == DOSING_PHASE_FINE ?
        DISPENSER_FLOW_FINE : DISPENSER_FLOW_FAST);
}

void dosing_controller_update(void)
{
    if (state == DOSING_STATE_DOSING) {
        int target = effective_target_grams();
        int current_weight = weight_sensor.read_grams();

        if (current_weight < 0 || current_weight > MAX_VALID_GRAMS) {
            return;
        }

        if (current_weight >= target) {
            dispenser.stop();
            state = DOSING_STATE_COMPLETED;
            return;
        }

        int remaining = target - current_weight;

        if (phase == DOSING_PHASE_FAST &&
            remaining <= FINE_THRESHOLD_GRAMS) {
            phase = DOSING_PHASE_FINE;

            if (dispenser.is_active()) {
                dispenser.start(DISPENSER_FLOW_FINE);
            }
        }

        int step = phase == DOSING_PHASE_FINE ?
            FINE_STEP_GRAMS : FAST_STEP_GRAMS;

        if (dispenser.is_active()) {
            weight_sensor.add_grams(step);
        }

        current_weight = weight_sensor.read_grams();

        if (current_weight < 0 || current_weight > MAX_VALID_GRAMS) {
            return;
        }

        if (current_weight >= target) {
            dispenser.stop();
            state = DOSING_STATE_COMPLETED;
        }

        return;
    }

    if (manual_release_active &&
        state != DOSING_STATE_DOSING) {
        weight_sensor.add_grams(MANUAL_STEP_GRAMS);
    }
}

void dosing_controller_cancel(void)
{
    dispenser.stop();

    manual_release_active = 0;

    state = DOSING_STATE_INTERRUPTED;
}

void dosing_controller_new_dosing(void)
{
    dispenser.stop();

    weight_sensor.reset();

    manual_release_active = 0;

    state = DOSING_STATE_IDLE;
}

void dosing_controller_manual_release_start(void)
{
    if (!dosing_controller_manual_release_is_allowed()) {
        return;
    }

    manual_release_active = 1;

    dispenser.start(DISPENSER_FLOW_MANUAL);
}

void dosing_controller_manual_release_stop(void)
{
    if (!manual_release_active) {
        return;
    }

    manual_release_active = 0;

    dispenser.stop();
}

int dosing_controller_manual_release_is_active(void)
{
    return manual_release_active;
}

int dosing_controller_manual_release_is_allowed(void)
{
    /* RS15: liberação manual disponível em Aguardando (IDLE),
       Concluído (COMPLETED) e Interrompido (INTERRUPTED);
       bloqueada apenas durante a dosagem automática (DOSING). */
    return state != DOSING_STATE_DOSING;
}

int dosing_controller_get_weight(void)
{
    return weight_sensor.read_grams();
}

int dosing_controller_get_target_grams(void)
{
    return effective_target_grams();
}

DosingState dosing_controller_get_state(void)
{
    return state;
}

DosingPhase dosing_controller_get_phase(void)
{
    return phase;
}
