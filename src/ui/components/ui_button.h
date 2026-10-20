#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct {
    lv_coord_t width;
    lv_coord_t height;
    lv_align_t align;
    lv_coord_t x;
    lv_coord_t y;
    lv_coord_t radius;
    lv_color_t color;
    lv_color_t text_color;
    int border;
    lv_color_t border_color;
    const char *text;
    const lv_font_t *font;
    lv_event_cb_t event_cb;
    void *user_data;
} UiButtonConfig;

lv_obj_t *ui_button_create(lv_obj_t *parent, const UiButtonConfig *config);

#ifdef __cplusplus
}
#endif

#endif
