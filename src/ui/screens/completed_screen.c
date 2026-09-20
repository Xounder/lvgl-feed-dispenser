#include "completed_screen.h"
#include "../screen_manager.h"
#include "../../domain/dosing_controller.h"

static void new_dosing_event_cb(lv_event_t *e)
{
    (void)e;

    screen_manager_show(SCREEN_MODE);
}

static void home_button_event_cb(lv_event_t *e)
{
    (void)e;

    screen_manager_show(SCREEN_HOME);
}

lv_obj_t *completed_screen_create(void)
{
    lv_obj_t *screen = lv_obj_create(NULL);

    DosingConfig *config = screen_manager_get_dosing_config();

    lv_obj_t *title = lv_label_create(screen);

    lv_label_set_text(
        title,
        "Dosagem concluida!"
    );

    lv_obj_align(
        title,
        LV_ALIGN_TOP_MID,
        0,
        60
    );

    lv_obj_t *weight_label = lv_label_create(screen);

    lv_label_set_text_fmt(
        weight_label,
        "%d g",
        config->target_grams
    );

    lv_obj_align(
        weight_label,
        LV_ALIGN_CENTER,
        0,
        -30
    );

    lv_obj_t *new_dosing_button = lv_button_create(screen);

    lv_obj_set_size(
        new_dosing_button,
        200,
        60
    );

    lv_obj_align(
        new_dosing_button,
        LV_ALIGN_CENTER,
        0,
        60
    );

    lv_obj_add_event_cb(
        new_dosing_button,
        new_dosing_event_cb,
        LV_EVENT_CLICKED,
        NULL
    );

    lv_obj_t *new_dosing_label =
        lv_label_create(new_dosing_button);

    lv_label_set_text(
        new_dosing_label,
        "Nova dosagem"
    );

    lv_obj_center(new_dosing_label);

    lv_obj_t *home_button = lv_button_create(screen);

    lv_obj_set_size(
        home_button,
        180,
        50
    );

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

    lv_label_set_text(
        home_label,
        "Voltar ao inicio"
    );

    lv_obj_center(home_label);

    return screen;
}