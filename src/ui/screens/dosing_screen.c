#include "dosing_screen.h"
#include "../screen_manager.h"
#include "../../domain/dosing_controller.h"

typedef struct {
    lv_obj_t *weight_label;
    lv_obj_t *progress_bar;
    lv_obj_t *status_label;
    lv_timer_t *timer;
    int target_grams;
} DosingScreenContext;

static void cancel_event_cb(lv_event_t *e)
{
    DosingScreenContext *context = lv_event_get_user_data(e);

    if (context->timer != NULL) {
        lv_timer_delete(context->timer);
        context->timer = NULL;
    }

    dosing_controller_cancel();

    screen_manager_show(SCREEN_INTERRUPTED);
}

static void emergency_event_cb(lv_event_t *e)
{
    DosingScreenContext *context = lv_event_get_user_data(e);

    /* Simulação do botão físico de emergência: mesma prioridade que Parar */
    if (context->timer != NULL) {
        lv_timer_delete(context->timer);
        context->timer = NULL;
    }

    dosing_controller_cancel();

    screen_manager_show(SCREEN_INTERRUPTED);
}

static void dosing_timer_cb(lv_timer_t *timer)
{
    DosingScreenContext *context = lv_timer_get_user_data(timer);

    dosing_controller_update();

    int current_weight = dosing_controller_get_weight();

    lv_label_set_text_fmt(
        context->weight_label,
        "Peso atual: %d g",
        current_weight
    );

    int progress = 0;

    if (context->target_grams > 0) {
        progress =
            (current_weight * 100) /
            context->target_grams;
    }

    if (progress > 100) {
        progress = 100;
    }

    lv_bar_set_value(
        context->progress_bar,
        progress,
        LV_ANIM_ON
    );

    if (dosing_controller_get_phase() == DOSING_PHASE_FINE) {
        lv_label_set_text(context->status_label, "Etapa fina: vazao reduzida");
    } else {
        lv_label_set_text(context->status_label, "Etapa rapida: vazao alta");
    }

    if (dosing_controller_get_state() == DOSING_STATE_COMPLETED) {
        lv_timer_delete(timer);
        context->timer = NULL;

        screen_manager_show(SCREEN_COMPLETED);

        return;
    }
}

lv_obj_t *dosing_screen_create(void)
{
    lv_obj_t *screen = lv_obj_create(NULL);

    DosingScreenContext *context =
        lv_malloc(sizeof(DosingScreenContext));

    context->target_grams = dosing_controller_get_target_grams();

    lv_obj_t *title = lv_label_create(screen);
    lv_label_set_text(title, "Dosando");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 40);

    lv_obj_t *target_label = lv_label_create(screen);

    lv_label_set_text_fmt(
        target_label,
        "Meta: %d g",
        context->target_grams
    );

    lv_obj_align(
        target_label,
        LV_ALIGN_CENTER,
        0,
        -100
    );

    context->weight_label = lv_label_create(screen);

    lv_label_set_text(
        context->weight_label,
        "Peso atual: 0 g"
    );

    lv_obj_align(
        context->weight_label,
        LV_ALIGN_CENTER,
        0,
        -50
    );

    context->progress_bar = lv_bar_create(screen);

    lv_obj_set_size(
        context->progress_bar,
        400,
        30
    );

    lv_obj_align(
        context->progress_bar,
        LV_ALIGN_CENTER,
        0,
        0
    );

    lv_bar_set_range(
        context->progress_bar,
        0,
        100
    );

    lv_bar_set_value(
        context->progress_bar,
        0,
        LV_ANIM_OFF
    );

    context->status_label = lv_label_create(screen);

    lv_label_set_text(
        context->status_label,
        "Iniciando..."
    );

    lv_obj_align(
        context->status_label,
        LV_ALIGN_CENTER,
        0,
        50
    );

    context->timer = lv_timer_create(
        dosing_timer_cb,
        300,
        context
    );

    /* Parar (comando na tela) — layout: "INTERROMPER DOSAGEM" */
    lv_obj_t *stop_button = lv_button_create(screen);

    lv_obj_set_size(stop_button, 210, 50);

    lv_obj_align(
        stop_button,
        LV_ALIGN_BOTTOM_LEFT,
        40,
        -30
    );

    lv_obj_add_event_cb(
        stop_button,
        cancel_event_cb,
        LV_EVENT_CLICKED,
        context
    );

    lv_obj_t *stop_label = lv_label_create(stop_button);
    lv_label_set_text(stop_label, "Interromper dosagem");
    lv_obj_center(stop_label);

    /* Emergência (simulação do botão físico) */
    lv_obj_t *emergency_button = lv_button_create(screen);

    lv_obj_set_size(emergency_button, 180, 50);

    lv_obj_align(
        emergency_button,
        LV_ALIGN_BOTTOM_MID,
        0,
        -30
    );

    lv_obj_add_event_cb(
        emergency_button,
        emergency_event_cb,
        LV_EVENT_CLICKED,
        context
    );

    lv_obj_t *emergency_label = lv_label_create(emergency_button);
    lv_label_set_text(emergency_label, "Emergencia");
    lv_obj_center(emergency_label);

    return screen;
}