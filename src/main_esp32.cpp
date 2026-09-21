#include <Arduino.h>

#include "lvgl/lvgl.h"

#include "hardware/esp32/board_display.h"
#include "hardware/esp32/real_weight_sensor.h"
#include "hardware/esp32/real_dispenser.h"
#include "ui/ui.h"

static lv_display_t *disp;

void setup(void)
{
    Serial.begin(115200);

    real_weight_sensor_init();
    real_dispenser_init();

    lv_init();
    disp = board_display_init();
    if (disp != NULL) {
        ui_init();
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