#include "manual_release_screen.h"
#include "../screen_manager.h"
#include "../components/ui_button.h"
#include "../components/manual_release_widget.h"
#include "../components/screen_chrome.h"
#include "../../domain/dosing_controller.h"

typedef struct {
    lv_obj_t *weight_label;
} ManualReleaseScreenContext;

static void back_event_cb(lv_event_t *e)
{
    (void)e;

    screen_manager_show(SCREEN_HOME);
}

static void tick_cb(void *user_data)
{
    ManualReleaseScreenContext *ctx = user_data;

    lv_label_set_text_fmt(
        ctx->weight_label,
        "%d g",
        dosing_controller_get_weight()
    );
}

lv_obj_t *manual_release_screen_create(void)
{
    lv_obj_t *screen = screen_chrome_create();

    ManualReleaseScreenContext *ctx =
        lv_malloc(sizeof(ManualReleaseScreenContext));

    screen_chrome_add_title(screen);
    screen_chrome_add_state(
        screen,
        "LIBERACAO MANUAL",
        "",
        CHROME_ACCENT_ORANGE
    );

    screen_chrome_add_subtitle(screen, "Pressione o botao para liberar", 136);

    lv_obj_t *weight_hint = lv_label_create(screen);
    lv_label_set_text(weight_hint, "Massa medida");
    lv_obj_set_style_text_color(weight_hint, CHROME_GREY, 0);
    lv_obj_set_style_text_font(weight_hint, &lv_font_montserrat_14, 0);
    lv_obj_align(weight_hint, LV_ALIGN_TOP_MID, 0, 210);

    ctx->weight_label = lv_label_create(screen);
    lv_label_set_text_fmt(
        ctx->weight_label,
        "%d g",
        dosing_controller_get_weight()
    );
    lv_obj_set_style_text_color(ctx->weight_label, CHROME_WHITE, 0);
    lv_obj_set_style_text_font(
        ctx->weight_label,
        &lv_font_montserrat_28,
        0
    );
    lv_obj_align(ctx->weight_label, LV_ALIGN_TOP_MID, 0, 228);

    manual_release_widget_create(
        screen,
        LV_ALIGN_BOTTOM_MID,
        0,
        -120,
        tick_cb,
        ctx
    );

    ui_button_create(screen, &(UiButtonConfig){
        .width = 440,
        .height = 44,
        .align = LV_ALIGN_BOTTOM_MID,
        .x = 0,
        .y = -70,
        .color = CHROME_BTN_DISABLED,
        .text_color = CHROME_GREY,
        .text = "VOLTAR",
        .font = &lv_font_montserrat_16,
        .event_cb = back_event_cb,
        .user_data = NULL
    });

    screen_chrome_add_bottom_nav(screen);

    return screen;
}
