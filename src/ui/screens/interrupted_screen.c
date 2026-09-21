#include "interrupted_screen.h"
#include "../screen_manager.h"
#include "manual_release_widget.h"
#include "../../domain/dosing_controller.h"

typedef struct {
    lv_obj_t *weight_label;
} InterruptedScreenContext;

static void new_dosing_event_cb(lv_event_t *e)
{
    (void)e;

    dosing_controller_new_dosing();

    screen_manager_show(SCREEN_MODE);
}

static void home_button_event_cb(lv_event_t *e)
{
    (void)e;

    dosing_controller_new_dosing();

    screen_manager_show(SCREEN_HOME);
}

static void tick_cb(void *user_data)
{
    InterruptedScreenContext *ctx = user_data;

    lv_label_set_text_fmt(
        ctx->weight_label,
        "Massa parcial: %d g",
        dosing_controller_get_weight()
    );
}

lv_obj_t *interrupted_screen_create(void)
{
    lv_obj_t *screen = lv_obj_create(NULL);

    InterruptedScreenContext *ctx =
        lv_malloc(sizeof(InterruptedScreenContext));

    lv_obj_t *title = lv_label_create(screen);
    lv_label_set_text(title, "Dosagem interrompida");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 60);

    ctx->weight_label = lv_label_create(screen);
    lv_label_set_text_fmt(
        ctx->weight_label,
        "Massa parcial: %d g",
        dosing_controller_get_weight()
    );
    lv_obj_align(ctx->weight_label, LV_ALIGN_CENTER, 0, -30);

    lv_obj_t *new_dosing_button = lv_button_create(screen);
    lv_obj_set_size(new_dosing_button, 200, 60);
    lv_obj_align(new_dosing_button, LV_ALIGN_CENTER, 0, 60);

    lv_obj_add_event_cb(
        new_dosing_button,
        new_dosing_event_cb,
        LV_EVENT_CLICKED,
        NULL
    );

    lv_obj_t *new_dosing_label = lv_label_create(new_dosing_button);
    lv_label_set_text(new_dosing_label, "Nova dosagem");
    lv_obj_center(new_dosing_label);

    lv_obj_t *home_button = lv_button_create(screen);
    lv_obj_set_size(home_button, 180, 50);
    lv_obj_align(home_button, LV_ALIGN_BOTTOM_MID, 0, -30);

    lv_obj_add_event_cb(
        home_button,
        home_button_event_cb,
        LV_EVENT_CLICKED,
        NULL
    );

    lv_obj_t *home_label = lv_label_create(home_button);
    lv_label_set_text(home_label, "Voltar ao inicio");
    lv_obj_center(home_label);

    /* Liberação manual disponível no estado Interrompido (RS14/RS16) */
    manual_release_widget_create(
        screen,
        LV_ALIGN_LEFT_MID,
        40,
        0,
        tick_cb,
        ctx
    );

    return screen;
}