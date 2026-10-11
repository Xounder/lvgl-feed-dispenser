#ifdef ROLE_EXTERNAL

#include <Arduino.h>

#include "hardware/esp32/real_weight_sensor.h"
#include "hardware/esp32/real_dispenser.h"
#include "hardware/esp32/real_buttons.h"
#include "hardware/esp32/real_indicator.h"
#include "hardware/esp32/physical_controls.h"
#include "link/link_backend.h"

void setup(void)
{
    Serial.begin(115200);

    real_weight_sensor_init();
    real_dispenser_init();
    real_buttons_init();
    real_indicator_init();
    physical_controls_init();

    link_backend_init();
}

void loop(void)
{
    physical_controls_poll();
    link_backend_poll();
    link_backend_tick();

    delay(5);
}

#endif
