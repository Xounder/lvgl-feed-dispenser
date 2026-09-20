#ifndef DOSING_CONTROLLER_H
#define DOSING_CONTROLLER_H

#include "dosing_config.h"

typedef enum {
    DOSING_STATE_IDLE,
    DOSING_STATE_DOSING,
    DOSING_STATE_COMPLETED
} DosingState;

void dosing_controller_init(DosingConfig *config);

void dosing_controller_start(void);

void dosing_controller_update(void);

void dosing_controller_cancel(void);

int dosing_controller_get_weight(void);

DosingState dosing_controller_get_state(void);

#endif