#include "dosing_screen.h"
#include "../screen_manager.h"
#include "screen_chrome.h"
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

    if (dosing_controller_get_state() == DOSING_STATE_COMPLETED) {
        lv_timer_delete(timer);
        context->timer = NULL;

        screen_manager_show(SCREEN_COMPLETED);

        return;
    }
}

lv_obj_t *dosing_screen_create(void)
{
    lv_obj_t *screen = screen_chrome_create();

    DosingScreenContext *context =
        lv_malloc(sizeof(DosingScreenContext));

    context->target_grams = dosing_controller_get_target_grams();

    screen_chrome_add_status_bar(screen);
    screen_chrome_add_title(screen);
    screen_chrome_add_state(
        screen,
        "DOSANDO",
        LV_SYMBOL_SETTINGS,
        CHROME_ACCENT_ORANGE
    );

    lv_obj_t *weight_hint = lv_label_create(screen);
    lv_label_set_text(weight_hint, "Massa atual");
    lv_obj_set_style_text_color(weight_hint, CHROME_GREY, 0);
    lv_obj_set_style_text_font(weight_hint, &lv_font_montserrat_14, 0);
    lv_obj_align(weight_hint, LV_ALIGN_TOP_MID, 0, 132);

    context->weight_label = lv_label_create(screen);
    lv_label_set_text(context->weight_label, "0 g");
    lv_obj_set_style_text_color(context->weight_label, CHROME_WHITE, 0);
    lv_obj_set_style_text_font(
        context->weight_label,
        &lv_font_montserrat_28,
        0
    );
    lv_obj_align(context->weight_label, LV_ALIGN_TOP_MID, 0, 150);

    lv_obj_t *progress_row = lv_obj_create(screen);
    lv_obj_set_size(progress_row, 440, 26);
    lv_obj_align(progress_row, LV_ALIGN_TOP_MID, 0, 204);
    lv_obj_clear_flag(progress_row, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(progress_row, 0, 0);
    lv_obj_set_style_bg_opa(progress_row, LV_OPA_TRANSP, 0);

    context->progress_bar = lv_bar_create(progress_row);
    lv_obj_set_size(context->progress_bar, 380, 18);
    lv_obj_align(context->progress_bar, LV_ALIGN_LEFT_MID, 0, 0);
    lv_bar_set_range(context->progress_bar, 0, 100);
    lv_bar_set_value(context->progress_bar, 0, LV_ANIM_OFF);
    lv_obj_set_style_bg_color(
        context->progress_bar,
        CHROME_BORDER,
        LV_PART_MAIN
    );
    lv_obj_set_style_bg_color(
        context->progress_bar,
        CHROME_ACCENT_ORANGE,
        LV_PART_INDICATOR
    );

    context->percent_label = lv_label_create(progress_row);
    lv_label_set_text(context->percent_label, "0%");
    lv_obj_set_style_text_color(context->percent_label, CHROME_WHITE, 0);
    lv_obj_set_style_text_font(
        context->percent_label,
        &lv_font_montserrat_14,
        0
    );
    lv_obj_align(context->percent_label, LV_ALIGN_RIGHT_MID, 0, 0);

    lv_obj_t *meta_row = lv_obj_create(screen);
    lv_obj_set_size(meta_row, 440, 24);
    lv_obj_align(meta_row, LV_ALIGN_TOP_MID, 0, 240);
    lv_obj_clear_flag(meta_row, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(meta_row, 0, 0);
    lv_obj_set_style_bg_opa(meta_row, LV_OPA_TRANSP, 0);

    lv_obj_t *meta_label = lv_label_create(meta_row);
    lv_label_set_text(meta_label, "Meta");
    lv_obj_set_style_text_color(meta_label, CHROME_GREY, 0);
    lv_obj_set_style_text_font(meta_label, &lv_font_montserrat_14, 0);
    lv_obj_align(meta_label, LV_ALIGN_LEFT_MID, 0, 0);

    lv_obj_t *target_label = lv_label_create(meta_row);
    lv_label_set_text_fmt(target_label, "%d g", context->target_grams);
    lv_obj_set_style_text_color(target_label, CHROME_GREY, 0);
    lv_obj_set_style_text_font(target_label, &lv_font_montserrat_14, 0);
    lv_obj_align(target_label, LV_ALIGN_RIGHT_MID, 0, 0);

    context->phase_label = lv_label_create(screen);
    lv_label_set_text(context->phase_label, "Iniciando...");
    lv_obj_set_style_text_color(context->phase_label, CHROME_GREY, 0);
    lv_obj_set_style_text_font(
        context->phase_label,
        &lv_font_montserrat_12,
        0
    );
    lv_obj_align(context->phase_label, LV_ALIGN_TOP_MID, 0, 270);

    lv_obj_t *stop_button = lv_button_create(screen);
    lv_obj_set_size(stop_button, 440, 42);
    lv_obj_align(stop_button, LV_ALIGN_TOP_MID, 0, 298);
    lv_obj_set_style_bg_color(stop_button, CHROME_RED_BTN, 0);
    lv_obj_set_style_radius(stop_button, 8, 0);

    lv_obj_add_event_cb(
        stop_button,
        cancel_event_cb,
        LV_EVENT_CLICKED,
        context
    );

    lv_obj_t *stop_label = lv_label_create(stop_button);
    lv_label_set_text(
        stop_label,
        LV_SYMBOL_STOP " INTERROMPER DOSAGEM"
    );
    lv_obj_set_style_text_color(stop_label, CHROME_WHITE, 0);
    lv_obj_set_style_text_font(stop_label, &lv_font_montserrat_16, 0);
    lv_obj_center(stop_label);

    lv_obj_t *manual_card = screen_chrome_add_card(screen, 376, 440, 80);
    lv_obj_set_style_bg_color(manual_card, CHROME_CARD_DISABLED, 0);
    lv_obj_set_style_border_color(manual_card, CHROME_BORDER, 0);

    lv_obj_t *manual_hint = lv_label_create(manual_card);
    lv_label_set_text(manual_hint, "LIBERACAO MANUAL DESABILITADA");
    lv_obj_set_style_text_color(manual_hint, CHROME_GREY, 0);
    lv_obj_set_style_text_font(manual_hint, &lv_font_montserrat_12, 0);
    lv_obj_align(manual_hint, LV_ALIGN_TOP_MID, 0, 10);

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