#include "config_screen.h"
#include "../screen_manager.h"

typedef struct {
    lv_obj_t *value_label;
    ConfigMode mode;
} ConfigScreenContext;

static void home_button_event_cb(lv_event_t *e)
{
    (void)e;

    screen_manager_show(SCREEN_HOME);
}

static void update_value_label(ConfigScreenContext *context)
{
    DosingConfig *config = screen_manager_get_dosing_config();

    if (context->mode == CONFIG_MODE_FIXED_AMOUNT) {
        lv_label_set_text_fmt(
            context->value_label,
            "%d g",
            config->target_grams
        );
    } else {
        lv_label_set_text_fmt(
            context->value_label,
            "%d porcao%s",
            config->portions,
            config->portions == 1 ? "" : "es"
        );
    }
}

static void decrease_value_event_cb(lv_event_t *e)
{
    ConfigScreenContext *context = lv_event_get_user_data(e);
    DosingConfig *config = screen_manager_get_dosing_config();

    if (context->mode == CONFIG_MODE_FIXED_AMOUNT) {
        if (config->target_grams > 10) {
            config->target_grams -= 10;
        }
    } else {
        if (config->portions > 1) {
            config->portions--;
        }
    }

    update_value_label(context);
}

static void increase_value_event_cb(lv_event_t *e)
{
    ConfigScreenContext *context = lv_event_get_user_data(e);
    DosingConfig *config = screen_manager_get_dosing_config();

    if (context->mode == CONFIG_MODE_FIXED_AMOUNT) {
        config->target_grams += 10;
    } else {
        config->portions++;
    }

    update_value_label(context);
}

static void continue_event_cb(lv_event_t *e)
{
    (void)e;

    screen_manager_show(SCREEN_DOSING);
}

lv_obj_t *config_screen_create(ConfigMode mode)
{
    lv_obj_t *screen = lv_obj_create(NULL);

    ConfigScreenContext *context = lv_malloc(sizeof(ConfigScreenContext));

    context->mode = mode;

    lv_obj_t *title = lv_label_create(screen);
    lv_label_set_text(title, "Configurar dosagem");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 40);

    lv_obj_t *mode_label = lv_label_create(screen);

    if (mode == CONFIG_MODE_FIXED_AMOUNT) {
        lv_label_set_text(mode_label, "Modo: Quantidade fixa");
    } else {
        lv_label_set_text(mode_label, "Modo: Porcoes");
    }

    lv_obj_align(mode_label, LV_ALIGN_TOP_MID, 0, 90);

    context->value_label = lv_label_create(screen);

    update_value_label(context);

    lv_obj_align(
        context->value_label,
        LV_ALIGN_CENTER,
        0,
        -20
    );

    lv_obj_t *decrease_button = lv_button_create(screen);
    lv_obj_set_size(decrease_button, 100, 60);
    lv_obj_align(
        decrease_button,
        LV_ALIGN_CENTER,
        -120,
        50
    );

    lv_obj_add_event_cb(
        decrease_button,
        decrease_value_event_cb,
        LV_EVENT_CLICKED,
        context
    );

    lv_obj_t *decrease_label = lv_label_create(decrease_button);
    lv_label_set_text(decrease_label, "-");
    lv_obj_center(decrease_label);

    lv_obj_t *increase_button = lv_button_create(screen);
    lv_obj_set_size(increase_button, 100, 60);
    lv_obj_align(
        increase_button,
        LV_ALIGN_CENTER,
        120,
        50
    );

    lv_obj_add_event_cb(
        increase_button,
        increase_value_event_cb,
        LV_EVENT_CLICKED,
        context
    );

    lv_obj_t *increase_label = lv_label_create(increase_button);
    lv_label_set_text(increase_label, "+");
    lv_obj_center(increase_label);

    lv_obj_t *continue_button = lv_button_create(screen);

    lv_obj_set_size(continue_button, 180, 60);

    lv_obj_align(
        continue_button,
        LV_ALIGN_BOTTOM_MID,
        0,
        -40
    );

    lv_obj_add_event_cb(
        continue_button,
        continue_event_cb,
        LV_EVENT_CLICKED,
        NULL
    );

    lv_obj_t *continue_label = lv_label_create(continue_button);
    lv_label_set_text(continue_label, "Continuar");
    lv_obj_center(continue_label);

    lv_obj_t *home_button = lv_button_create(screen);

    lv_obj_set_size(home_button, 160, 50);

    lv_obj_align(
        home_button,
        LV_ALIGN_BOTTOM_LEFT,
        30,
        -30
    );

    lv_obj_add_event_cb(
        home_button,
        home_button_event_cb,
        LV_EVENT_CLICKED,
        NULL
    );

    lv_obj_t *home_label = lv_label_create(home_button);
    lv_label_set_text(home_label, "Inicio");
    lv_obj_center(home_label);

    return screen;
}