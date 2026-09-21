#include "manual_release_widget.h"
#include "../../domain/dosing_controller.h"

#define MANUAL_RELEASE_TICK_MS 200

static void update_led(ManualReleaseWidget *widget)
{
    if (dosing_controller_manual_release_is_active()) {
        lv_obj_set_style_bg_color(
            widget->led,
            lv_palette_main(LV_PALETTE_GREEN),
            0
        );
    } else {
        lv_obj_set_style_bg_color(
            widget->led,
            lv_palette_main(LV_PALETTE_GREY),
            0
        );
    }
}

void manual_release_widget_run_tick(ManualReleaseWidget *widget)
{
    dosing_controller_update();

    update_led(widget);

    if (widget->tick_cb != NULL) {
        widget->tick_cb(widget->user_data);
    }

    if (!dosing_controller_manual_release_is_active() &&
        widget->timer != NULL) {
        lv_timer_delete(widget->timer);
        widget->timer = NULL;
    }
}

static void timer_cb(lv_timer_t *timer)
{
    ManualReleaseWidget *widget = lv_timer_get_user_data(timer);

    manual_release_widget_run_tick(widget);
}

static void press_event_cb(lv_event_t *e)
{
    ManualReleaseWidget *widget = lv_event_get_user_data(e);

    if (!dosing_controller_manual_release_is_allowed()) {
        return;
    }

    dosing_controller_manual_release_start();

    update_led(widget);

    if (widget->tick_cb != NULL) {
        widget->tick_cb(widget->user_data);
    }

    if (widget->timer == NULL) {
        widget->timer = lv_timer_create(
            timer_cb,
            MANUAL_RELEASE_TICK_MS,
            widget
        );
    }
}

static void release_event_cb(lv_event_t *e)
{
    ManualReleaseWidget *widget = lv_event_get_user_data(e);

    dosing_controller_manual_release_stop();

    update_led(widget);

    if (widget->tick_cb != NULL) {
        widget->tick_cb(widget->user_data);
    }

    if (widget->timer != NULL) {
        lv_timer_delete(widget->timer);
        widget->timer = NULL;
    }
}

ManualReleaseWidget *manual_release_widget_create(
    lv_obj_t *parent,
    lv_align_t align,
    lv_coord_t x,
    lv_coord_t y,
    ManualReleaseTickCb tick_cb,
    void *user_data
)
{
    ManualReleaseWidget *widget = lv_malloc(sizeof(ManualReleaseWidget));

    widget->timer = NULL;
    widget->tick_cb = tick_cb;
    widget->user_data = user_data;

    lv_obj_t *col = lv_obj_create(parent);
    lv_obj_set_size(col, 260, 150);
    lv_obj_clear_flag(col, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(col, align, x, y);
    lv_obj_set_style_border_width(col, 0, 0);
    lv_obj_set_style_bg_opa(col, LV_OPA_TRANSP, 0);

    lv_obj_t *led_row = lv_obj_create(col);
    lv_obj_set_size(led_row, 240, 50);
    lv_obj_clear_flag(led_row, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(led_row, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_border_width(led_row, 0, 0);
    lv_obj_set_style_bg_opa(led_row, LV_OPA_TRANSP, 0);

    widget->led = lv_obj_create(led_row);
    lv_obj_set_size(widget->led, 30, 30);
    lv_obj_align(widget->led, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_bg_color(
        widget->led,
        lv_palette_main(LV_PALETTE_GREY),
        0
    );
    lv_obj_set_style_radius(widget->led, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_border_width(widget->led, 1, 0);

    lv_obj_t *led_label = lv_label_create(led_row);
    lv_label_set_text(led_label, "LED: modo manual");
    lv_obj_align(led_label, LV_ALIGN_LEFT_MID, 45, 0);

    lv_obj_t *button = lv_button_create(col);
    lv_obj_set_size(button, 250, 60);
    lv_obj_align(button, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_add_event_cb(
        button,
        press_event_cb,
        LV_EVENT_PRESSED,
        widget
    );

    lv_obj_add_event_cb(
        button,
        release_event_cb,
        LV_EVENT_RELEASED,
        widget
    );

    lv_obj_t *label = lv_label_create(button);
    lv_label_set_text(label, "Liberacao manual");
    lv_obj_center(label);

    return widget;
}