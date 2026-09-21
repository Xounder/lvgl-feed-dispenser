#include "screen_chrome.h"

/* Barra de status (topo, simulando o relogio/sinal do mockup) */
void screen_chrome_add_status_bar(lv_obj_t *screen)
{
    lv_obj_t *bar = lv_obj_create(screen);
    lv_obj_set_size(bar, 480, 24);
    lv_obj_align(bar, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_clear_flag(bar, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(bar, 0, 0);
    lv_obj_set_style_bg_opa(bar, LV_OPA_TRANSP, 0);

    lv_obj_t *time_label = lv_label_create(bar);
    lv_label_set_text(time_label, "10:30");
    lv_obj_set_style_text_color(time_label, CHROME_WHITE, 0);
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_12, 0);
    lv_obj_align(time_label, LV_ALIGN_LEFT_MID, 10, 0);

    lv_obj_t *signal_label = lv_label_create(bar);
    lv_label_set_text(
        signal_label,
        LV_SYMBOL_WIFI " " LV_SYMBOL_BATTERY_FULL " 100%"
    );
    lv_obj_set_style_text_color(signal_label, CHROME_WHITE, 0);
    lv_obj_set_style_text_font(signal_label, &lv_font_montserrat_12, 0);
    lv_obj_align(signal_label, LV_ALIGN_RIGHT_MID, -10, 0);
}

/* Título da aplicacao + divisor */
void screen_chrome_add_title(lv_obj_t *screen)
{
    lv_obj_t *title = lv_label_create(screen);
    lv_label_set_text(title, "Pesagem e Dosagem");
    lv_obj_set_style_text_color(title, CHROME_WHITE, 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 28);

    lv_obj_t *divider = lv_obj_create(screen);
    lv_obj_set_size(divider, 440, 2);
    lv_obj_align(divider, LV_ALIGN_TOP_MID, 0, 58);
    lv_obj_clear_flag(divider, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(divider, 0, 0);
    lv_obj_set_style_bg_color(divider, CHROME_BORDER, 0);
}

/* Bloco "ESTADO ATUAL" + estado em destaque com cor de acento */
void screen_chrome_add_state(lv_obj_t *screen, const char *state, const char *icon, lv_color_t accent)
{
    lv_obj_t *hint = lv_label_create(screen);
    lv_label_set_text(hint, "ESTADO ATUAL");
    lv_obj_set_style_text_color(hint, CHROME_GREY, 0);
    lv_obj_set_style_text_font(hint, &lv_font_montserrat_14, 0);
    lv_obj_align(hint, LV_ALIGN_TOP_MID, 0, 68);

    lv_obj_t *state_label = lv_label_create(screen);
    lv_label_set_text_fmt(state_label, "%s %s", state, icon);
    lv_obj_set_style_text_color(state_label, accent, 0);
    lv_obj_set_style_text_font(state_label, &lv_font_montserrat_28, 0);
    lv_obj_align(state_label, LV_ALIGN_TOP_MID, 0, 92);
}

void screen_chrome_add_subtitle(lv_obj_t *screen, const char *text, lv_coord_t y)
{
    lv_obj_t *subtitle = lv_label_create(screen);
    lv_label_set_text(subtitle, text);
    lv_obj_set_style_text_color(subtitle, CHROME_WHITE, 0);
    lv_obj_set_style_text_font(subtitle, &lv_font_montserrat_16, 0);
    lv_obj_align(subtitle, LV_ALIGN_TOP_MID, 0, y);
}

/* Barra inferior: Inicio | Dosagens | Historico | Config. */
void screen_chrome_add_bottom_nav(lv_obj_t *screen)
{
    static const char *icons[4] = {
        LV_SYMBOL_HOME,
        LV_SYMBOL_LIST,
        LV_SYMBOL_REFRESH,
        LV_SYMBOL_SETTINGS
    };
    static const char *labels[4] = {
        "Inicio",
        "Dosagens",
        "Historico",
        "Config."
    };

    lv_obj_t *nav = lv_obj_create(screen);
    lv_obj_set_size(nav, 480, 56);
    lv_obj_align(nav, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_clear_flag(nav, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(nav, 1, 0);
    lv_obj_set_style_border_color(nav, CHROME_BORDER, 0);
    lv_obj_set_style_border_side(nav, LV_BORDER_SIDE_TOP, 0);
    lv_obj_set_style_bg_color(nav, lv_color_hex(0x111111), 0);

    for (int i = 0; i < 4; i++) {
        lv_obj_t *item = lv_obj_create(nav);
        lv_obj_set_size(item, 120, 56);
        lv_obj_clear_flag(item, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_border_width(item, 0, 0);
        lv_obj_set_style_bg_opa(item, LV_OPA_TRANSP, 0);
        lv_obj_align(item, LV_ALIGN_CENTER, (i * 120) - 180, 0);

        lv_color_t color;

        if (i == 0) {
            color = CHROME_BLUE;
        } else {
            color = CHROME_GREY;
        }

        lv_obj_t *icon = lv_label_create(item);
        lv_label_set_text(icon, icons[i]);
        lv_obj_set_style_text_color(icon, color, 0);
        lv_obj_set_style_text_font(icon, &lv_font_montserrat_18, 0);
        lv_obj_align(icon, LV_ALIGN_TOP_MID, 0, 4);

        lv_obj_t *label = lv_label_create(item);
        lv_label_set_text(label, labels[i]);
        lv_obj_set_style_text_color(label, color, 0);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
        lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 38);
    }
}

/* Container padrão de card (tela preta, fundo cinza escuro, borda/raio) */
lv_obj_t *screen_chrome_add_card(lv_obj_t *screen, lv_coord_t y, lv_coord_t w, lv_coord_t h)
{
    lv_obj_t *card = lv_obj_create(screen);
    lv_obj_set_size(card, w, h);
    lv_obj_align(card, LV_ALIGN_TOP_MID, 0, y);
    lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(card, CHROME_CARD, 0);
    lv_obj_set_style_radius(card, 12, 0);
    lv_obj_set_style_border_width(card, 2, 0);
    lv_obj_set_style_border_color(card, CHROME_BORDER, 0);

    return card;
}

lv_obj_t *screen_chrome_create(void)
{
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, CHROME_BG, 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(screen, 0, 0);
    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    return screen;
}