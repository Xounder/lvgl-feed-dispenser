#include "mode_screen.h"
#include "../screen_manager.h"

static void home_button_event_cb(lv_event_t *e)
{
    (void)e;

    screen_manager_show(SCREEN_HOME);
}

static void fixed_amount_event_cb(lv_event_t *e)
{
    (void)e;

    screen_manager_show_config(CONFIG_MODE_FIXED_AMOUNT);
}

static void portions_event_cb(lv_event_t *e)
{
    (void)e;

    screen_manager_show_config(CONFIG_MODE_PORTIONS);
}

lv_obj_t *mode_screen_create(void)
{
    lv_obj_t *screen = lv_obj_create(NULL);

    lv_obj_t *title = lv_label_create(screen);
    lv_label_set_text(title, "Selecione o modo");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 40);

    /* Quantidade fixa */
    lv_obj_t *fixed_button = lv_button_create(screen);
    lv_obj_set_size(fixed_button, 250, 60);
    lv_obj_align(fixed_button, LV_ALIGN_CENTER, 0, -30);

    lv_obj_add_event_cb(
        fixed_button,
        fixed_amount_event_cb,
        LV_EVENT_CLICKED,
        NULL
    );

    lv_obj_t *fixed_label = lv_label_create(fixed_button);
    lv_label_set_text(fixed_label, "Quantidade fixa");
    lv_obj_center(fixed_label);

    /* Porções */
    lv_obj_t *portions_button = lv_button_create(screen);
    lv_obj_set_size(portions_button, 250, 60);
    lv_obj_align(portions_button, LV_ALIGN_CENTER, 0, 50);

    lv_obj_add_event_cb(
        portions_button,
        portions_event_cb,
        LV_EVENT_CLICKED,
        NULL
    );

    lv_obj_t *portions_label = lv_label_create(portions_button);
    lv_label_set_text(portions_label, "Porcoes");
    lv_obj_center(portions_label);

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