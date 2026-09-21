#include "config_screen.h"
#include "../screen_manager.h"
#include "screen_chrome.h"
#include "../../domain/dosing_controller.h"

#define CHIP_W 170
#define CHIP_H 34

typedef struct {
    lv_obj_t *value_label;
    lv_obj_t *equiv_label;
    DosingMode mode;
} ConfigScreenContext;

static void home_button_event_cb(lv_event_t *e)
{
    (void)e;

    screen_manager_show(SCREEN_HOME);
}

static void grams_tab_event_cb(lv_event_t *e)
{
    (void)e;

    screen_manager_show_config(DOSING_MODE_GRAMS);
}

static void currency_tab_event_cb(lv_event_t *e)
{
    (void)e;

    screen_manager_show_config(DOSING_MODE_CURRENCY);
}

static void update_value_label(ConfigScreenContext *context)
{
    DosingConfig *config = screen_manager_get_dosing_config();

    if (context->mode == DOSING_MODE_GRAMS) {
        lv_label_set_text_fmt(
            context->value_label,
            "%d g",
            config->target_grams
        );
        lv_label_set_text(context->equiv_label, "");
    } else {
        lv_label_set_text_fmt(
            context->value_label,
            "R$ %d,%02d",
            config->target_money_cents / 100,
            config->target_money_cents % 100
        );
        lv_label_set_text_fmt(
            context->equiv_label,
            "Equivale a %d g",
            dosing_controller_get_target_grams()
        );
    }
}

static void set_chip_value(ConfigScreenContext *context, int value)
{
    DosingConfig *config = screen_manager_get_dosing_config();

    if (context->mode == DOSING_MODE_GRAMS) {
        config->target_grams = value;
    } else {
        config->target_money_cents = value;
    }

    update_value_label(context);
}

static void decrease_value_event_cb(lv_event_t *e)
{
    ConfigScreenContext *context = lv_event_get_user_data(e);
    DosingConfig *config = screen_manager_get_dosing_config();

    if (context->mode == DOSING_MODE_GRAMS) {
        if (config->target_grams > 10) {
            config->target_grams -= 10;
        }
    } else {
        if (config->target_money_cents > 50) {
            config->target_money_cents -= 50;
        }
    }

    update_value_label(context);
}

static void increase_value_event_cb(lv_event_t *e)
{
    ConfigScreenContext *context = lv_event_get_user_data(e);
    DosingConfig *config = screen_manager_get_dosing_config();

    if (context->mode == DOSING_MODE_GRAMS) {
        config->target_grams += 10;
    } else {
        config->target_money_cents += 50;
    }

    update_value_label(context);
}

static void continue_event_cb(lv_event_t *e)
{
    (void)e;

    screen_manager_show(SCREEN_DOSING);
}

static lv_obj_t *create_tab(
    lv_obj_t *screen,
    lv_coord_t x,
    const char *text,
    int active,
    lv_event_cb_t event_cb
)
{
    lv_obj_t *tab = lv_button_create(screen);
    lv_obj_set_size(tab, 240, 34);
    lv_obj_align(tab, LV_ALIGN_TOP_MID, x, 120);
    lv_obj_set_style_radius(tab, 8, 0);

    lv_obj_add_event_cb(tab, event_cb, LV_EVENT_CLICKED, NULL);

    if (active) {
        lv_obj_set_style_bg_color(tab, CHROME_BLUE, 0);
    } else {
        lv_obj_set_style_bg_color(tab, CHROME_BG, 0);
        lv_obj_set_style_border_width(tab, 1, 0);
        lv_obj_set_style_border_color(tab, CHROME_BORDER, 0);
    }

    lv_obj_t *label = lv_label_create(tab);
    lv_label_set_text(label, text);

    if (active) {
        lv_obj_set_style_text_color(label, CHROME_WHITE, 0);
    } else {
        lv_obj_set_style_text_color(label, CHROME_GREY, 0);
    }

    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);
    lv_obj_center(label);

    return tab;
}

static void chip_event_cb(lv_event_t *e);

static lv_obj_t *create_chip(
    lv_obj_t *screen,
    lv_coord_t x,
    lv_coord_t y,
    const char *text,
    ConfigScreenContext *context,
    int value
)
{
    lv_obj_t *chip = lv_button_create(screen);
    lv_obj_set_size(chip, CHIP_W, CHIP_H);
    lv_obj_align(chip, LV_ALIGN_TOP_MID, x, y);
    lv_obj_set_style_bg_color(chip, CHROME_CARD, 0);
    lv_obj_set_style_radius(chip, 8, 0);
    lv_obj_set_style_border_width(chip, 1, 0);
    lv_obj_set_style_border_color(chip, CHROME_BORDER, 0);

    lv_obj_add_event_cb(chip, chip_event_cb, LV_EVENT_CLICKED, context);

    lv_obj_t *label = lv_label_create(chip);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, CHROME_WHITE, 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_14, 0);
    lv_obj_center(label);

    lv_obj_set_user_data(chip, (void *)(lv_intptr_t)value);

    return chip;
}

static void chip_event_cb(lv_event_t *e)
{
    ConfigScreenContext *context = lv_event_get_user_data(e);

    lv_obj_t *chip = lv_event_get_target(e);

    set_chip_value(
        context,
        (int)(lv_intptr_t)lv_obj_get_user_data(chip)
    );
}

static void create_chip_grid(
    lv_obj_t *screen,
    ConfigScreenContext *context
)
{
    static const int grams_values[6] = {
        100, 250, 500, 750, 1000, 2000
    };
    static const int currency_values[6] = {
        500, 1000, 2000, 5000, 10000, 20000
    };

    const int *values;

    if (context->mode == DOSING_MODE_GRAMS) {
        values = grams_values;
    } else {
        values = currency_values;
    }

    for (int i = 0; i < 6; i++) {
        int col = i % 3;
        int row = i / 3;
        lv_coord_t x = (col - 1) * (CHIP_W + 12);
        lv_coord_t y = 254 + row * (CHIP_H + 6);

        char text[24];

        if (context->mode == DOSING_MODE_GRAMS) {
            if (values[i] >= 1000) {
                lv_snprintf(
                    text,
                    sizeof(text),
                    "%d.%03d g",
                    values[i] / 1000,
                    values[i] % 1000
                );
            } else {
                lv_snprintf(text, sizeof(text), "%d g", values[i]);
            }
        } else {
            lv_snprintf(
                text,
                sizeof(text),
                "R$ %d,%02d",
                values[i] / 100,
                values[i] % 100
            );
        }

        create_chip(screen, x, y, text, context, values[i]);
    }
}

lv_obj_t *config_screen_create(DosingMode mode)
{
    lv_obj_t *screen = screen_chrome_create();

    ConfigScreenContext *context = lv_malloc(sizeof(ConfigScreenContext));

    context->mode = mode;

    screen_chrome_add_status_bar(screen);
    screen_chrome_add_title(screen);

    lv_obj_t *state_label = lv_label_create(screen);
    lv_label_set_text(state_label, "AGUARDANDO");
    lv_obj_set_style_text_color(state_label, CHROME_BLUE, 0);
    lv_obj_set_style_text_font(state_label, &lv_font_montserrat_20, 0);
    lv_obj_align(state_label, LV_ALIGN_TOP_MID, 0, 66);

    screen_chrome_add_subtitle(screen, "Defina a quantidade", 96);

    create_tab(
        screen,
        -130,
        "MASSA (g)",
        mode == DOSING_MODE_GRAMS,
        grams_tab_event_cb
    );
    create_tab(
        screen,
        130,
        "VALOR (R$)",
        mode == DOSING_MODE_CURRENCY,
        currency_tab_event_cb
    );

    lv_obj_t *quantity_hint = lv_label_create(screen);
    lv_label_set_text(quantity_hint, "Quantidade desejada");
    lv_obj_set_style_text_color(quantity_hint, CHROME_WHITE, 0);
    lv_obj_set_style_text_font(quantity_hint, &lv_font_montserrat_14, 0);
    lv_obj_align(quantity_hint, LV_ALIGN_TOP_MID, 0, 162);

    context->value_label = lv_label_create(screen);
    context->equiv_label = lv_label_create(screen);

    lv_obj_set_style_text_color(context->value_label, CHROME_WHITE, 0);
    lv_obj_set_style_text_font(
        context->value_label,
        &lv_font_montserrat_24,
        0
    );
    lv_obj_align(context->value_label, LV_ALIGN_TOP_MID, 0, 186);

    lv_obj_set_style_text_color(context->equiv_label, CHROME_GREY, 0);
    lv_obj_set_style_text_font(
        context->equiv_label,
        &lv_font_montserrat_12,
        0
    );
    lv_obj_align(context->equiv_label, LV_ALIGN_TOP_MID, 0, 218);

    update_value_label(context);

    lv_obj_t *decrease_button = lv_button_create(screen);
    lv_obj_set_size(decrease_button, 90, 40);
    lv_obj_align(decrease_button, LV_ALIGN_TOP_MID, -212, 184);
    lv_obj_set_style_bg_color(decrease_button, CHROME_CARD, 0);
    lv_obj_set_style_radius(decrease_button, 8, 0);
    lv_obj_add_event_cb(
        decrease_button,
        decrease_value_event_cb,
        LV_EVENT_CLICKED,
        context
    );

    lv_obj_t *decrease_label = lv_label_create(decrease_button);
    lv_label_set_text(decrease_label, LV_SYMBOL_MINUS);
    lv_obj_set_style_text_color(decrease_label, CHROME_WHITE, 0);
    lv_obj_center(decrease_label);

    lv_obj_t *increase_button = lv_button_create(screen);
    lv_obj_set_size(increase_button, 90, 40);
    lv_obj_align(increase_button, LV_ALIGN_TOP_MID, 212, 184);
    lv_obj_set_style_bg_color(increase_button, CHROME_CARD, 0);
    lv_obj_set_style_radius(increase_button, 8, 0);
    lv_obj_add_event_cb(
        increase_button,
        increase_value_event_cb,
        LV_EVENT_CLICKED,
        context
    );

    lv_obj_t *increase_label = lv_label_create(increase_button);
    lv_label_set_text(increase_label, LV_SYMBOL_PLUS);
    lv_obj_set_style_text_color(increase_label, CHROME_WHITE, 0);
    lv_obj_center(increase_label);

    lv_obj_t *quick_hint = lv_label_create(screen);
    lv_label_set_text(quick_hint, "Valores rapidos");
    lv_obj_set_style_text_color(quick_hint, CHROME_GREY, 0);
    lv_obj_set_style_text_font(quick_hint, &lv_font_montserrat_12, 0);
    lv_obj_align(quick_hint, LV_ALIGN_TOP_MID, 0, 236);

    create_chip_grid(screen, context);

    lv_obj_t *continue_button = lv_button_create(screen);
    lv_obj_set_size(continue_button, 560, 40);
    lv_obj_align(continue_button, LV_ALIGN_TOP_MID, 0, 332);
    lv_obj_set_style_bg_color(continue_button, CHROME_GREEN_BTN, 0);
    lv_obj_set_style_radius(continue_button, 8, 0);
    lv_obj_add_event_cb(
        continue_button,
        continue_event_cb,
        LV_EVENT_CLICKED,
        NULL
    );

    lv_obj_t *continue_label = lv_label_create(continue_button);
    lv_label_set_text(
        continue_label,
        LV_SYMBOL_PLAY " INICIAR DOSAGEM AUTOMATICA"
    );
    lv_obj_set_style_text_color(continue_label, CHROME_WHITE, 0);
    lv_obj_set_style_text_font(continue_label, &lv_font_montserrat_16, 0);
    lv_obj_center(continue_label);

    lv_obj_t *voltar_button = lv_button_create(screen);
    lv_obj_set_size(voltar_button, 560, 32);
    lv_obj_align(voltar_button, LV_ALIGN_TOP_MID, 0, 380);
    lv_obj_set_style_bg_color(voltar_button, CHROME_BTN_DISABLED, 0);
    lv_obj_set_style_radius(voltar_button, 8, 0);
    lv_obj_add_event_cb(
        voltar_button,
        home_button_event_cb,
        LV_EVENT_CLICKED,
        NULL
    );

    lv_obj_t *voltar_label = lv_label_create(voltar_button);
    lv_label_set_text(voltar_label, "VOLTAR");
    lv_obj_set_style_text_color(voltar_label, CHROME_GREY, 0);
    lv_obj_set_style_text_font(voltar_label, &lv_font_montserrat_14, 0);
    lv_obj_center(voltar_label);

    screen_chrome_add_bottom_nav(screen);

    return screen;
}