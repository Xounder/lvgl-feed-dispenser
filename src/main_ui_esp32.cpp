#ifdef ROLE_SCREEN

#include <Arduino.h>

#include "lvgl.h"

#include "hardware/esp32/board_display.h"
#include "ui/ui.h"
#include "link/link_proxy.h"

static lv_display_t *disp;
static lv_obj_t *link_overlay;
static lv_obj_t *link_overlay_label;

static void link_overlay_create(void)
{
    link_overlay = lv_obj_create(lv_layer_top());
    lv_obj_remove_style_all(link_overlay);
    lv_obj_set_size(link_overlay, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(link_overlay, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(link_overlay, LV_OPA_70, 0);
    lv_obj_clear_flag(link_overlay, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(link_overlay, LV_OBJ_FLAG_HIDDEN);

    link_overlay_label = lv_label_create(link_overlay);
    lv_obj_center(link_overlay_label);
    lv_obj_set_style_text_color(link_overlay_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(link_overlay_label, &lv_font_montserrat_24, 0);
}

static void link_overlay_update(void)
{
    if (link_overlay == NULL) {
        return;
    }

    if (!link_proxy_is_up()) {
        lv_label_set_text(link_overlay_label, "SEM COMUNICACAO");
        lv_obj_clear_flag(link_overlay, LV_OBJ_FLAG_HIDDEN);
    } else if (link_proxy_error() != 0) {
        lv_label_set_text(link_overlay_label, "ERRO DO SENSOR DE PESO");
        lv_obj_clear_flag(link_overlay, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(link_overlay, LV_OBJ_FLAG_HIDDEN);
    }
}

void setup(void)
{
    Serial.begin(115200);

    lv_init();

    disp = board_display_init();

    if (disp != NULL) {
        ui_init();
        link_overlay_create();
    } else {
        Serial.println("display nao inicializado; UI indisponivel");
    }
}

void loop(void)
{
    link_proxy_poll();

    if (disp != NULL) {
        link_overlay_update();
        lv_timer_handler();
    }

    delay(5);
}

#endif
