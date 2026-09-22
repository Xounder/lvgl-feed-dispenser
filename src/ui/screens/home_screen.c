#include "home_screen.h"
#include "../screen_manager.h"
#include "screen_chrome.h"
#include "manual_release_widget.h"

static void grams_card_event_cb(lv_event_t *e)
{
    (void)e;

    screen_manager_show_config(DOSING_MODE_GRAMS);
}

static void currency_card_event_cb(lv_event_t *e)
{
    (void)e;

    screen_manager_show_config(DOSING_MODE_CURRENCY);
}

static lv_obj_t *create_mode_card(
    lv_obj_t *screen,
    lv_coord_t y,
    const char *title,
    const char *subtitle,
    lv_event_cb_t event_cb
)
{
    lv_obj_t *card = screen_chrome_add_card(screen, y, 440, 76);

    lv_obj_add_flag(card, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(card, event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *title_label = lv_label_create(card);
    lv_label_set_text(title_label, title);
    lv_obj_set_style_text_color(title_label, CHROME_WHITE, 0);
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_20, 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_LEFT, 20, 14);

    lv_obj_t *sub_label = lv_label_create(card);
    lv_label_set_text(sub_label, subtitle);
    lv_obj_set_style_text_color(sub_label, CHROME_GREY, 0);
    lv_obj_set_style_text_font(sub_label, &lv_font_montserrat_14, 0);
    lv_obj_align(sub_label, LV_ALIGN_TOP_LEFT, 20, 46);

    return card;
}

lv_obj_t *home_screen_create(void)
{
    lv_obj_t *screen = screen_chrome_create();

    screen_chrome_add_title(screen);
    screen_chrome_add_state(screen, "AGUARDANDO", "", CHROME_BLUE);
    screen_chrome_add_subtitle(screen, "Selecione o modo de dosagem", 136);

    create_mode_card(
        screen,
        176,
        "MASSA",
        "Dosar por peso (g)",
        grams_card_event_cb
    );

    create_mode_card(
        screen,
        260,
        "VALOR MONETARIO",
        "Dosar por valor (R$)",
        currency_card_event_cb
    );

    manual_release_widget_create(
        screen,
        LV_ALIGN_BOTTOM_MID,
        0,
        -60,
        NULL,
        NULL
    );

    screen_chrome_add_bottom_nav(screen);

    return screen;
}