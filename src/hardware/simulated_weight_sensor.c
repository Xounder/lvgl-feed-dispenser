#include "weight_sensor.h"

static int simulated_weight = 0;

static int simulated_read_grams(void)
{
    return simulated_weight;
}

static void simulated_add_grams(int grams)
{
    simulated_weight += grams;
}

static void simulated_reset(void)
{
    simulated_weight = 0;
}

WeightSensor simulated_weight_sensor = {
    .read_grams = simulated_read_grams,
    .add_grams = simulated_add_grams,
    .reset = simulated_reset
};