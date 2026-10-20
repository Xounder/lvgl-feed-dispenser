#ifndef UI_METRIC_H
#define UI_METRIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct {
    lv_obj_t *hint;
    lv_obj_t *value;
} UiMetric;

void ui_metric_create(
    lv_obj_t *parent,
    const char *hint,
    lv_coord_t hint_y,
    lv_coord_t value_y,
    lv_coord_t value_x,
    UiMetric *out
);

#ifdef __cplusplus
}
#endif

#endif
