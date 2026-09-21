#include "board_display.h"

#include <Arduino.h>
#include "esp_timer.h"

#include "board_config.h"

static uint32_t board_tick_ms(void)
{
    return (uint32_t)(esp_timer_get_time() / 1000);
}

lv_display_t *board_display_init(void)
{
    lv_tick_set_cb(board_tick_ms);

    /*
     * Integrar aqui o driver do display do fabricante
     * (ex.: exemplo LVGL_Arduino para o 4,3" 800x480 RGB + GT911,
     * com lv_display_create/register_lv_display e indev de touch).
     * Enquanto pendente, retorna NULL e a UI nao e iniciada.
     */
    return NULL;
}