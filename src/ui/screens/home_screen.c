#include "home_screen.h"
#include "../screen_manager.h"

static void start_button_event_cb(lv_event_t *e)
{
    (void)e;

    screen_manager_show(SCREEN_MODE);
}

lv_obj_t *home_screen_create(void)
{
    lv_obj_t *screen = lv_obj_create(NULL);

    /* Título */
    lv_obj_t *title = lv_label_create(screen);
    lv_label_set_text(title, "Dosador de Racao");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 40);

    /* Estado */
    lv_obj_t *status = lv_label_create(screen);
    lv_label_set_text(status, "Pronto");
    lv_obj_align(status, LV_ALIGN_CENTER, 0, -30);

    /* Botão */
    lv_obj_t *button = lv_button_create(screen);
    lv_obj_set_size(button, 200, 60);
    lv_obj_align(button, LV_ALIGN_CENTER, 0, 50);

    lv_obj_add_event_cb(button, start_button_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *button_label = lv_label_create(button);
    lv_label_set_text(button_label, "Iniciar");
    lv_obj_center(button_label);

    return screen;
}