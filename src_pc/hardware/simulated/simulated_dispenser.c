#include "../../../src/hardware/dispenser.h"

static int active = 0;

static void simulated_start(void)
{
    active = 1;
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