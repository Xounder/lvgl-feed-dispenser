#include "interrupted_screen.h"
#include "../screen_manager.h"
#include "../components/ui_button.h"
#include "../components/ui_meta_row.h"
#include "../components/ui_progress.h"
#include "../components/ui_metric.h"
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

    UiMetric metric;

    ui_metric_create(screen, "Massa parcial", 128, 148, 0, &metric);
    ctx->weight_label = metric.value;
    lv_label_set_text_fmt(ctx->weight_label, "%d g", partial_grams);

    UiMetaRow meta;

    ui_meta_row_create(screen, "Meta", 192, &meta);
    lv_label_set_text_fmt(meta.value, "%d g", target_grams);

    UiProgress progress;

    ui_progress_create(screen, 222, CHROME_ACCENT_RED, &progress);
    ctx->progress_bar = progress.bar;
    ctx->percent_label = progress.percent;
    lv_label_set_text_fmt(ctx->percent_label, "%d%%", percent);
    lv_bar_set_value(ctx->progress_bar, percent, LV_ANIM_OFF);

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