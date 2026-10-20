#include "interrupted_screen.h"
#include "../screen_manager.h"
#include "../components/ui_button.h"
#include "../components/screen_chrome.h"
#include "../components/manual_release_widget.h"
#include "../../domain/dosing_controller.h"

typedef struct {
    lv_obj_t *weight_label;
    lv_obj_t *progress_bar;
    lv_obj_t *percent_label;
} InterruptedScreenContext;

static void new_dosing_event_cb(lv_event_t *e)
{
    (void)e;

    dosing_controller_new_dosing();

    screen_manager_show(SCREEN_HOME);
}

static void tick_cb(void *user_data)
{
    InterruptedScreenContext *ctx = user_data;

    int weight = dosing_controller_get_weight();
    int target = dosing_controller_get_target_grams();
    int percent = 0;

    if (target > 0) {
        percent = (weight * 100) / target;
    }

    if (percent > 100) {
        percent = 100;
    }

    lv_label_set_text_fmt(ctx->weight_label, "%d g", weight);
    lv_label_set_text_fmt(ctx->percent_label, "%d%%", percent);
    lv_bar_set_value(ctx->progress_bar, percent, LV_ANIM_OFF);
}

lv_obj_t *interrupted_screen_create(void)
{
    lv_obj_t *screen = screen_chrome_create();

    InterruptedScreenContext *ctx =
        lv_malloc(sizeof(InterruptedScreenContext));

    int partial_grams = dosing_controller_get_weight();
    int target_grams = dosing_controller_get_target_grams();

    int percent = 0;

    if (target_grams > 0) {
        percent = (partial_grams * 100) / target_grams;
    }

    if (percent > 100) {
        percent = 100;
    }

    screen_chrome_add_title(screen);
    screen_chrome_add_state(
        screen,
        "INTERROMPIDO",
        LV_SYMBOL_WARNING,
        CHROME_ACCENT_RED
    );

    lv_obj_t *partial_hint = lv_label_create(screen);
    lv_label_set_text(partial_hint, "Massa parcial");
    lv_obj_set_style_text_color(partial_hint, CHROME_GREY, 0);
    lv_obj_set_style_text_font(partial_hint, &lv_font_montserrat_14, 0);
    lv_obj_align(partial_hint, LV_ALIGN_TOP_MID, 0, 128);

    ctx->weight_label = lv_label_create(screen);
    lv_label_set_text_fmt(ctx->weight_label, "%d g", partial_grams);
    lv_obj_set_style_text_color(ctx->weight_label, CHROME_WHITE, 0);
    lv_obj_set_style_text_font(
        ctx->weight_label,
        &lv_font_montserrat_28,
        0
    );
    lv_obj_align(ctx->weight_label, LV_ALIGN_TOP_MID, 0, 148);

    lv_obj_t *meta_row = lv_obj_create(screen);
    lv_obj_set_size(meta_row, 440, 24);
    lv_obj_align(meta_row, LV_ALIGN_TOP_MID, 0, 192);
    lv_obj_clear_flag(meta_row, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(meta_row, 0, 0);
    lv_obj_set_style_bg_opa(meta_row, LV_OPA_TRANSP, 0);

    lv_obj_t *meta_label = lv_label_create(meta_row);
    lv_label_set_text(meta_label, "Meta");
    lv_obj_set_style_text_color(meta_label, CHROME_GREY, 0);
    lv_obj_set_style_text_font(meta_label, &lv_font_montserrat_14, 0);
    lv_obj_align(meta_label, LV_ALIGN_LEFT_MID, 0, 0);

    lv_obj_t *target_label = lv_label_create(meta_row);
    lv_label_set_text_fmt(target_label, "%d g", target_grams);
    lv_obj_set_style_text_color(target_label, CHROME_GREY, 0);
    lv_obj_set_style_text_font(target_label, &lv_font_montserrat_14, 0);
    lv_obj_align(target_label, LV_ALIGN_RIGHT_MID, 0, 0);

    lv_obj_t *progress_row = lv_obj_create(screen);
    lv_obj_set_size(progress_row, 440, 26);
    lv_obj_align(progress_row, LV_ALIGN_TOP_MID, 0, 222);
    lv_obj_clear_flag(progress_row, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(progress_row, 0, 0);
    lv_obj_set_style_bg_opa(progress_row, LV_OPA_TRANSP, 0);

    ctx->progress_bar = lv_bar_create(progress_row);
    lv_obj_set_size(ctx->progress_bar, 380, 18);
    lv_obj_align(ctx->progress_bar, LV_ALIGN_LEFT_MID, 0, 0);
    lv_bar_set_range(ctx->progress_bar, 0, 100);
    lv_bar_set_value(ctx->progress_bar, percent, LV_ANIM_OFF);
    lv_obj_set_style_bg_color(
        ctx->progress_bar,
        CHROME_BORDER,
        LV_PART_MAIN
    );
    lv_obj_set_style_bg_color(
        ctx->progress_bar,
        CHROME_ACCENT_RED,
        LV_PART_INDICATOR
    );

    ctx->percent_label = lv_label_create(progress_row);
    lv_label_set_text_fmt(ctx->percent_label, "%d%%", percent);
    lv_obj_set_style_text_color(ctx->percent_label, CHROME_GREY, 0);
    lv_obj_set_style_text_font(
        ctx->percent_label,
        &lv_font_montserrat_14,
        0
    );
    lv_obj_align(ctx->percent_label, LV_ALIGN_RIGHT_MID, 0, 0);

    UiButtonConfig new_dosing_config = {
        .width = 440,
        .height = 46,
        .align = LV_ALIGN_TOP_MID,
        .x = 0,
        .y = 292,
        .color = CHROME_BTN_BLUE,
        .text_color = CHROME_WHITE,
        .text = LV_SYMBOL_REFRESH " NOVA DOSAGEM",
        .font = &lv_font_montserrat_16,
        .event_cb = new_dosing_event_cb,
        .user_data = NULL
    };

    ui_button_create(screen, &new_dosing_config);

    manual_release_widget_create(
        screen,
        LV_ALIGN_BOTTOM_MID,
        0,
        -60,
        tick_cb,
        ctx
    );

    screen_chrome_add_bottom_nav(screen);

    return screen;
}