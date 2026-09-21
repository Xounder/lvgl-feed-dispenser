#include "mode_screen.h"
#include "../screen_manager.h"

static void home_button_event_cb(lv_event_t *e)
{
    (void)e;

    screen_manager_show(SCREEN_HOME);
}

static void grams_event_cb(lv_event_t *e)
{
    (void)e;

    screen_manager_show_config(DOSING_MODE_GRAMS);
}

static void currency_event_cb(lv_event_t *e)
{
    (void)e;

    screen_manager_show_config(DOSING_MODE_CURRENCY);
}

lv_obj_t *mode_screen_create(void)
{
    lv_obj_t *screen = lv_obj_create(NULL);

    lv_obj_t *title = lv_label_create(screen);
    lv_label_set_text(title, "Selecione o modo");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 40);

    /* Massa */
    lv_obj_t *grams_button = lv_button_create(screen);
    lv_obj_set_size(grams_button, 250, 60);
    lv_obj_align(grams_button, LV_ALIGN_CENTER, 0, -30);

    lv_obj_add_event_cb(
        grams_button,
        grams_event_cb,
        LV_EVENT_CLICKED,
        NULL
    );

    lv_obj_t *grams_label = lv_label_create(grams_button);
    lv_label_set_text(grams_label, "Massa");
    lv_obj_center(grams_label);

    /* Valor (R$) */
    lv_obj_t *currency_button = lv_button_create(screen);
    lv_obj_set_size(currency_button, 250, 60);
    lv_obj_align(currency_button, LV_ALIGN_CENTER, 0, 50);

    lv_obj_add_event_cb(
        currency_button,
        currency_event_cb,
        LV_EVENT_CLICKED,
        NULL
    );

    lv_obj_t *currency_label = lv_label_create(currency_button);
    lv_label_set_text(currency_label, "Valor (R$)");
    lv_obj_center(currency_label);

    lv_obj_t *home_button = lv_button_create(screen);

    lv_obj_set_size(home_button, 160, 50);

    lv_obj_align(
        home_button,
        LV_ALIGN_BOTTOM_MID,
        0,
        -30
    );

    lv_obj_add_event_cb(
        home_button,
        home_button_event_cb,
        LV_EVENT_CLICKED,
        NULL
    );

    lv_obj_t *home_label = lv_label_create(home_button);
    lv_label_set_text(home_label, "Voltar ao inicio");
    lv_obj_center(home_label);

    return screen;
}