#include "completed_screen.h"
#include "../screen_manager.h"
#include "../components/ui_button.h"
#include "../components/ui_meta_row.h"
#include "../components/ui_metric.h"
#include "../components/screen_chrome.h"
#include "../components/manual_release_widget.h"
#include "../../domain/dosing_controller.h"

typedef struct {
    lv_obj_t *weight_label;
} CompletedScreenContext;

static void new_dosing_event_cb(lv_event_t *e)
{
    (void)e;

    dosing_controller_new_dosing();

    screen_manager_show(SCREEN_HOME);
}

static void tick_cb(void *user_data)
{
    CompletedScreenContext *ctx = user_data;

    lv_label_set_text_fmt(
        ctx->weight_label,
        "%d g",
        dosing_controller_get_weight()
    );
}

lv_obj_t *completed_screen_create(void)
{
    lv_obj_t *screen = screen_chrome_create();

    CompletedScreenContext *ctx =
        lv_malloc(sizeof(CompletedScreenContext));

    int final_grams = dosing_controller_get_weight();
    int target_grams = dosing_controller_get_target_grams();

    screen_chrome_add_title(screen);
    screen_chrome_add_state(
        screen,
        "CONCLUIDO",
        LV_SYMBOL_OK,
        CHROME_ACCENT_GREEN
    );

    UiMetric metric;

    ui_metric_create(screen, "Massa final", 132, 150, -40, &metric);
    ctx->weight_label = metric.value;
    lv_label_set_text_fmt(ctx->weight_label, "%d g", final_grams);

    lv_obj_t *check_icon = lv_label_create(screen);
    lv_label_set_text(check_icon, LV_SYMBOL_OK);
    lv_obj_set_style_text_color(check_icon, CHROME_ACCENT_GREEN, 0);
    lv_obj_set_style_text_font(check_icon, &lv_font_montserrat_28, 0);
    lv_obj_align(check_icon, LV_ALIGN_TOP_MID, 100, 150);

    UiMetaRow meta;

    ui_meta_row_create(screen, "Meta", 200, &meta);
    lv_label_set_text_fmt(meta.value, "%d g", target_grams);

    UiButtonConfig new_dosing_config = {
        .width = 440,
        .height = 46,
        .align = LV_ALIGN_TOP_MID,
        .x = 0,
        .y = 244,
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