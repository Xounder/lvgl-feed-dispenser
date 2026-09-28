#include "real_weight_sensor.h"

#include <Arduino.h>
#include "HX711.h"

#include "../weight_sensor.h"
#include "board_config.h"

static HX711 scale;
static int ready = 0;

static int real_read_grams(void)
{
    if (!ready) {
        return 0;
    }
    return (int)scale.get_units(10);
}

static void real_add_grams(int grams)
{
    (void)grams;
}

static void real_reset(void)
{
    if (ready) {
        scale.tare();
    }
}

WeightSensor weight_sensor = {
    real_read_grams,
    real_add_grams,
    real_reset
};

void real_weight_sensor_init(void)
{
    scale.begin(BOARD_HX711_DATA_PIN, BOARD_HX711_SCK_PIN);
    if (!scale.wait_ready_timeout(1000, 10)) {
        Serial.println("HX711 nao detectado; dosagem sem peso real");
        return;
    }
    scale.set_scale(BOARD_HX711_CALIBRATION_FACTOR);
    scale.tare(10);
    ready = 1;
}