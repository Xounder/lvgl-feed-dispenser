#include "../../../src/hardware/dispenser.h"

static int active = 0;

static void simulated_start(DispenserFlow flow)
{
    active = flow == DISPENSER_FLOW_FAST ||
        flow == DISPENSER_FLOW_FINE ||
        flow == DISPENSER_FLOW_MANUAL;
}

static void simulated_stop(void)
{
    active = 0;
}

static int simulated_is_active(void)
{
    return active;
}

Dispenser dispenser = {
    .start = simulated_start,
    .stop = simulated_stop,
    .is_active = simulated_is_active
};
