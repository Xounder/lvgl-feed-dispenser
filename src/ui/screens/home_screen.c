#include "home_screen.h"
#include "../screen_manager.h"
#include "manual_release_widget.h"
#include "../../domain/dosing_controller.h"

typedef struct {
    lv_obj_t *weight_label;
} HomeScreenContext;

static void tick_cb(void *user_data)
{
    HomeScreenContext *ctx = user_data;

    int grams = dosing_controller_get_weight();

    if (dosing_controller_manual_release_is_active()) {
        lv_label_set_text_fmt(
            ctx->weight_label,
            "Liberando manualmente... %d g",
            grams
        );
    } else {
        lv_label_set_text_fmt(
            ctx->weight_label,
            "Peso atual: %d g",
            grams
        );
    }
}

static void start_button_event_cb(lv_event_t *e)
{
    (void)e;

    screen_manager_show(SCREEN_MODE);
}

lv_obj_t *home_screen_create(void)
{
    lv_obj_t *screen = lv_obj_create(NULL);

    HomeScreenContext *ctx = lv_malloc(sizeof(HomeScreenContext));

    lv_obj_t *title = lv_label_create(screen);
    lv_label_set_text(title, "Dosador de Racao");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 30);

    lv_obj_t *subtitle = lv_label_create(screen);
    lv_label_set_text(subtitle, "Aguardando dosagem");
    lv_obj_align(subtitle, LV_ALIGN_TOP_MID, 0, 70);

    ctx->weight_label = lv_label_create(screen);
    lv_label_set_text(ctx->weight_label, "Peso atual: 0 g");

    lv_obj_align(
        ctx->weight_label,
        LV_ALIGN_CENTER,
        0,
        -100
    );

    /* Liberação manual (LED + botão de segurar) */
    manual_release_widget_create(
        screen,
        LV_ALIGN_CENTER,
        0,
        20,
        tick_cb,
        ctx
    );

    /* Botão iniciar */
    lv_obj_t *start_button = lv_button_create(screen);
    lv_obj_set_size(start_button, 200, 60);
    lv_obj_align(start_button, LV_ALIGN_BOTTOM_MID, 0, -30);

    lv_obj_add_event_cb(start_button, start_button_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *start_label = lv_label_create(start_button);
    lv_label_set_text(start_label, "Iniciar");
    lv_obj_center(start_label);

    return screen;
}