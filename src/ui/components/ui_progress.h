#ifndef UI_PROGRESS_H
#define UI_PROGRESS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct {
    lv_obj_t *bar;
    lv_obj_t *percent;
} UiProgress;

void ui_progress_create(
    lv_obj_t *parent,
    lv_coord_t y,
    lv_color_t indicator_color,
    UiProgress *out
);

#ifdef __cplusplus
}
#endif

#endif
