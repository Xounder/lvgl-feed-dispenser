#include <Arduino.h>

#include "lvgl.h"

#include "hardware/esp32/board_display.h"
#include "hardware/esp32/real_weight_sensor.h"
#include "hardware/esp32/real_dispenser.h"
#include "hardware/esp32/real_buttons.h"
#include "hardware/esp32/real_indicator.h"
#include "hardware/esp32/physical_controls.h"
#include "ui/ui.h"

static lv_display_t *disp;

void setup(void)
{
    Serial.begin(115200);
    delay(200);

    Serial.printf(
        "[SYS] boot psram=%u/%u heap=%u\n",
        (unsigned)ESP.getFreePsram(),
        (unsigned)ESP.getPsramSize(),
        (unsigned)ESP.getFreeHeap()
    );

    Serial.println("[SYS] A peso");

    real_weight_sensor_init();
    real_weight_sensor_probe("pos-init");
    Serial.println("[SYS] B servo");

    real_dispenser_init();
    real_weight_sensor_probe("pos-servo");
    Serial.println("[SYS] C botoes/led");

    real_buttons_init();
    real_indicator_init();
    Serial.println("[SYS] C2 controles fisicos");

    physical_controls_init();
    real_weight_sensor_probe("pos-ctrl");
    Serial.println("[SYS] H dominio pronto");

    lv_init();
    Serial.println("[SYS] D lv_init");

    disp = board_display_init();
    Serial.println("[SYS] E display");

    if (disp != NULL) {
        real_weight_sensor_probe("pos-display");
        real_weight_sensor_init();
        real_weight_sensor_probe("pos-reinit");
        Serial.println("[SYS] F ui_init");
        ui_init();
        real_weight_sensor_probe("pos-ui");
        Serial.println("[SYS] G pronto");
    } else {
        Serial.println(
            "[SYS] display nao inicializado; botoes fisicos seguem ativos"
        );
    }
}

void loop(void)
{
    physical_controls_poll();

    real_weight_sensor_log();

    if (disp != NULL) {
        lv_timer_handler();
    }
    delay(5);
}