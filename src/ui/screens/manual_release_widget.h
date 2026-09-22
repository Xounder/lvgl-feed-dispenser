#ifndef MANUAL_RELEASE_WIDGET_H
#define MANUAL_RELEASE_WIDGET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

typedef void (*ManualReleaseTickCb)(void *user_data);

typedef struct ManualReleaseWidget {
    lv_timer_t *timer;
    ManualReleaseTickCb tick_cb;
    void *user_data;
} ManualReleaseWidget;

ManualReleaseWidget *manual_release_widget_create(
    lv_obj_t *parent,
    lv_align_t align,
    lv_coord_t x,
    lv_coord_t y,
    ManualReleaseTickCb tick_cb,
    void *user_data
);

void manual_release_widget_run_tick(ManualReleaseWidget *widget);

#ifdef __cplusplus
}
#endif

#endif