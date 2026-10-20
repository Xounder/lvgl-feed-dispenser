#ifndef UI_META_ROW_H
#define UI_META_ROW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct {
    lv_obj_t *value;
} UiMetaRow;

void ui_meta_row_create(
    lv_obj_t *parent,
    const char *label,
    lv_coord_t y,
    UiMetaRow *out
);

#ifdef __cplusplus
}
#endif

#endif
