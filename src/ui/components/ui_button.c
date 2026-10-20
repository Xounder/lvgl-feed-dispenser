#include "ui_button.h"

lv_obj_t *ui_button_create(lv_obj_t *parent, const UiButtonConfig *config)
{
    lv_obj_t *button = lv_button_create(parent);

    lv_obj_set_size(button, config->width, config->height);
    lv_obj_align(button, config->align, config->x, config->y);
    lv_obj_set_style_bg_color(button, config->color, 0);
    lv_obj_set_style_radius(
        button,
        config->radius > 0 ? config->radius : 8,
        0
    );

    if (config->border) {
        lv_obj_set_style_border_width(button, 1, 0);
        lv_obj_set_style_border_color(button, config->border_color, 0);
    }

    if (config->event_cb != NULL) {
        lv_obj_add_event_cb(
            button,
            config->event_cb,
            LV_EVENT_CLICKED,
            config->user_data
        );
    }

    if (config->text != NULL) {
        lv_obj_t *label = lv_label_create(button);

        lv_label_set_text(label, config->text);
        lv_obj_set_style_text_color(label, config->text_color, 0);

        if (config->font != NULL) {
            lv_obj_set_style_text_font(label, config->font, 0);
        }

        lv_obj_center(label);
    }

    return button;
}
