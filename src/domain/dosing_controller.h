#ifndef DOSING_CONTROLLER_H
#define DOSING_CONTROLLER_H

#include "dosing_config.h"

typedef enum {
    DOSING_STATE_IDLE,
    DOSING_STATE_DOSING,
    DOSING_STATE_COMPLETED,
    DOSING_STATE_INTERRUPTED
} DosingState;

typedef enum {
    DOSING_PHASE_FAST,
    DOSING_PHASE_FINE
} DosingPhase;

void dosing_controller_init(DosingConfig *config);

void dosing_controller_start(void);

void dosing_controller_update(void);

void dosing_controller_cancel(void);

void dosing_controller_new_dosing(void);

void dosing_controller_manual_release_start(void);

void dosing_controller_manual_release_stop(void);

int dosing_controller_manual_release_is_active(void);

int dosing_controller_manual_release_is_allowed(void);

int dosing_controller_get_weight(void);

int dosing_controller_get_target_grams(void);

DosingState dosing_controller_get_state(void);

DosingPhase dosing_controller_get_phase(void);

#endif