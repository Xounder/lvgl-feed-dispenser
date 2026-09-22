#include "completed_screen.h"
#include "../screen_manager.h"
#include "screen_chrome.h"
#include "manual_release_widget.h"
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

    lv_obj_t *final_hint = lv_label_create(screen);
    lv_label_set_text(final_hint, "Massa final");
    lv_obj_set_style_text_color(final_hint, CHROME_GREY, 0);
    lv_obj_set_style_text_font(final_hint, &lv_font_montserrat_14, 0);
    lv_obj_align(final_hint, LV_ALIGN_TOP_MID, 0, 132);

    ctx->weight_label = lv_label_create(screen);
    lv_label_set_text_fmt(ctx->weight_label, "%d g", final_grams);
    lv_obj_set_style_text_color(ctx->weight_label, CHROME_WHITE, 0);
    lv_obj_set_style_text_font(
        ctx->weight_label,
        &lv_font_montserrat_28,
        0
    );
    lv_obj_align(ctx->weight_label, LV_ALIGN_TOP_MID, -40, 150);

    lv_obj_t *check_icon = lv_label_create(screen);
    lv_label_set_text(check_icon, LV_SYMBOL_OK);
    lv_obj_set_style_text_color(check_icon, CHROME_ACCENT_GREEN, 0);
    lv_obj_set_style_text_font(check_icon, &lv_font_montserrat_28, 0);
    lv_obj_align(check_icon, LV_ALIGN_TOP_MID, 100, 150);

    lv_obj_t *meta_row = lv_obj_create(screen);
    lv_obj_set_size(meta_row, 440, 24);
    lv_obj_align(meta_row, LV_ALIGN_TOP_MID, 0, 200);
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

    lv_obj_t *new_dosing_button = lv_button_create(screen);
    lv_obj_set_size(new_dosing_button, 440, 46);
    lv_obj_align(new_dosing_button, LV_ALIGN_TOP_MID, 0, 244);
    lv_obj_set_style_bg_color(new_dosing_button, CHROME_BTN_BLUE, 0);
    lv_obj_set_style_radius(new_dosing_button, 8, 0);

    lv_obj_add_event_cb(
        new_dosing_button,
        new_dosing_event_cb,
        LV_EVENT_CLICKED,
        NULL
    );

    lv_obj_t *new_dosing_label = lv_label_create(new_dosing_button);
    lv_label_set_text(new_dosing_label, LV_SYMBOL_REFRESH " NOVA DOSAGEM");
    lv_obj_set_style_text_color(new_dosing_label, CHROME_WHITE, 0);
    lv_obj_set_style_text_font(new_dosing_label, &lv_font_montserrat_16, 0);
    lv_obj_center(new_dosing_label);

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