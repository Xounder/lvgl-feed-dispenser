#include "real_indicator.h"

#include <Arduino.h>

#include "../indicator.h"
#include "board_config.h"

static void real_set(int on)
{
    digitalWrite(
        BOARD_LED_PIN,
        on ? BOARD_LED_ACTIVE_LEVEL : !BOARD_LED_ACTIVE_LEVEL
    );
}

Indicator indicator = {
    real_set
};

void real_indicator_init(void)
{
    pinMode(BOARD_LED_PIN, OUTPUT);
    real_set(0);
}
