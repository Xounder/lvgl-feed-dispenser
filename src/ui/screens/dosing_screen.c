#include "dosing_screen.h"
#include "../screen_manager.h"
#include "../components/ui_button.h"
#include "../components/ui_meta_row.h"
#include "../components/ui_progress.h"
#include "../components/ui_metric.h"
#include "../components/screen_chrome.h"
#include "../../domain/dosing_controller.h"

typedef struct {
    lv_obj_t *weight_label;
    lv_obj_t *progress_bar;
    lv_obj_t *percent_label;
    lv_obj_t *phase_label;
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

static void dosing_timer_cb(lv_timer_t *timer)
{
    DosingScreenContext *context = lv_timer_get_user_data(timer);

    dosing_controller_update();

    int current_weight = dosing_controller_get_weight();

    lv_label_set_text_fmt(
        context->weight_label,
        "%d g",
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

    lv_label_set_text_fmt(
        context->percent_label,
        "%d%%",
        progress
    );

    if (dosing_controller_get_phase() == DOSING_PHASE_FINE) {
        lv_label_set_text(
            context->phase_label,
            "Etapa fina: vazao reduzida"
        );
    } else {
        lv_label_set_text(
            context->phase_label,
            "Etapa rapida: vazao alta"
        );
    }

    DosingState state = dosing_controller_get_state();

    if (state == DOSING_STATE_COMPLETED) {
        lv_timer_delete(timer);
        context->timer = NULL;

        screen_manager_show(SCREEN_COMPLETED);

        return;
    }

    if (state == DOSING_STATE_INTERRUPTED) {
        lv_timer_delete(timer);
        context->timer = NULL;

        screen_manager_show(SCREEN_INTERRUPTED);

        return;
    }
}

lv_obj_t *dosing_screen_create(void)
{
    lv_obj_t *screen = screen_chrome_create();

    DosingScreenContext *context =
        lv_malloc(sizeof(DosingScreenContext));

    context->target_grams = dosing_controller_get_target_grams();

    screen_chrome_add_title(screen);
    screen_chrome_add_state(
        screen,
        "DOSANDO",
        LV_SYMBOL_SETTINGS,
        CHROME_ACCENT_ORANGE
    );

    UiMetric metric;

    ui_metric_create(screen, "Massa atual", 132, 150, 0, &metric);
    context->weight_label = metric.value;

    UiProgress progress;

    ui_progress_create(screen, 204, CHROME_ACCENT_ORANGE, &progress);
    context->progress_bar = progress.bar;
    context->percent_label = progress.percent;

    UiMetaRow meta;

    ui_meta_row_create(screen, "Meta", 240, &meta);
    lv_label_set_text_fmt(meta.value, "%d g", context->target_grams);

    context->phase_label = lv_label_create(screen);
    lv_label_set_text(context->phase_label, "Iniciando...");
    lv_obj_set_style_text_color(context->phase_label, CHROME_GREY, 0);
    lv_obj_set_style_text_font(
        context->phase_label,
        &lv_font_montserrat_12,
        0
    );
    lv_obj_align(context->phase_label, LV_ALIGN_TOP_MID, 0, 270);

    UiButtonConfig stop_config = {
        .width = 440,
        .height = 42,
        .align = LV_ALIGN_TOP_MID,
        .x = 0,
        .y = 298,
        .color = CHROME_RED_BTN,
        .text_color = CHROME_WHITE,
        .text = LV_SYMBOL_STOP " INTERROMPER DOSAGEM",
        .font = &lv_font_montserrat_16,
        .event_cb = cancel_event_cb,
        .user_data = context
    };

    ui_button_create(screen, &stop_config);

    lv_obj_t *manual_card = screen_chrome_add_card(screen, 660, 440, 80);
    lv_obj_set_style_bg_color(manual_card, CHROME_CARD_DISABLED, 0);
    lv_obj_set_style_border_color(manual_card, CHROME_BORDER, 0);

    lv_obj_t *manual_button = lv_obj_create(manual_card);
    lv_obj_set_size(manual_button, 400, 36);
    lv_obj_align(manual_button, LV_ALIGN_BOTTOM_MID, 0, -8);
    lv_obj_clear_flag(manual_button, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(manual_button, CHROME_BTN_DISABLED, 0);
    lv_obj_set_style_radius(manual_button, 8, 0);

    lv_obj_t *manual_label = lv_label_create(manual_button);
    lv_label_set_text(manual_label, "LIBERACAO MANUAL");
    lv_obj_set_style_text_color(manual_label, CHROME_DARK_GREY, 0);
    lv_obj_set_style_text_font(manual_label, &lv_font_montserrat_14, 0);
    lv_obj_center(manual_label);

    context->timer = lv_timer_create(
        dosing_timer_cb,
        300,
        context
    );

    screen_chrome_add_bottom_nav(screen);

    return screen;
}