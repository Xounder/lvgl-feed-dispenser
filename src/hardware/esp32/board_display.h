#ifndef BOARD_DISPLAY_H
#define BOARD_DISPLAY_H

#include "lvgl/lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

lv_display_t *board_display_init(void);

#ifdef __cplusplus
}
#endif

#endif