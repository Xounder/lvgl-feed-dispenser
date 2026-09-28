#include <Arduino.h>

#include "lvgl.h"

#include "hardware/esp32/board_display.h"
#include "hardware/esp32/real_weight_sensor.h"
#include "hardware/esp32/real_dispenser.h"
#include "ui/ui.h"

static lv_display_t *disp;

void setup(void)
{
    Serial.begin(115200);

    Serial.println("A");

    real_weight_sensor_init();
    Serial.println("B");

    real_dispenser_init();
    Serial.println("C");

    lv_init();
    Serial.println("D");

    disp = board_display_init();
    Serial.println("E");

    if (disp != NULL) {
        Serial.println("F");
        ui_init();
        Serial.println("G");
    } else {
        Serial.println("display nao inicializado; aguardando driver do fabricante");
    }
}

void loop(void)
{
    if (disp != NULL) {
        lv_timer_handler();
    }
    delay(5);
}