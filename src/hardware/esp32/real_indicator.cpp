#include "real_indicator.h"

#include <Arduino.h>

#include "../indicator.h"
#include "board_config.h"

static int s_on = -1;

static void real_set(int on)
{
    digitalWrite(
        BOARD_LED_PIN,
        on ? BOARD_LED_ACTIVE_LEVEL : !BOARD_LED_ACTIVE_LEVEL
    );

    if (on != s_on) {
        s_on = on;

        Serial.printf(
            "[LED] %s GPIO%d\n",
            on ? "LIGADO" : "DESLIGADO",
            BOARD_LED_PIN
        );
    }
}

Indicator indicator = {
    real_set
};

void real_indicator_init(void)
{
    pinMode(BOARD_LED_PIN, OUTPUT);
    real_set(0);
}
