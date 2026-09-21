#ifndef SCREEN_CHROME_H
#define SCREEN_CHROME_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

/* Cores canonicas do layout (mockups .images) */
#define CHROME_BG            lv_color_hex(0x050505)
#define CHROME_CARD          lv_color_hex(0x1A1A1A)
#define CHROME_CARD_DISABLED lv_color_hex(0x0D0D0D)
#define CHROME_BORDER        lv_color_hex(0x333333)
#define CHROME_WHITE         lv_color_white()
#define CHROME_GREY          lv_color_hex(0x9E9E9E)
#define CHROME_DARK_GREY     lv_color_hex(0x616161)
#define CHROME_BTN_DISABLED  lv_color_hex(0x2A2A2A)
#define CHROME_BLUE          lv_color_hex(0x29A0F5)
#define CHROME_BTN_BLUE      lv_color_hex(0x1565C0)
#define CHROME_ACCENT_GREEN  lv_color_hex(0x4CAF50)
#define CHROME_GREEN_BTN     lv_color_hex(0x2E7D32)
#define CHROME_ACCENT_ORANGE lv_color_hex(0xFF9800)
#define CHROME_ACCENT_RED    lv_color_hex(0xFF5252)
#define CHROME_RED_BTN       lv_color_hex(0x8B0000)

lv_obj_t *screen_chrome_create(void);

void screen_chrome_add_status_bar(lv_obj_t *screen);

void screen_chrome_add_title(lv_obj_t *screen);

void screen_chrome_add_state(lv_obj_t *screen, const char *state, const char *icon, lv_color_t accent);

void screen_chrome_add_subtitle(lv_obj_t *screen, const char *text, lv_coord_t y);

void screen_chrome_add_bottom_nav(lv_obj_t *screen);

lv_obj_t *screen_chrome_add_card(lv_obj_t *screen, lv_coord_t y, lv_coord_t w, lv_coord_t h);

#ifdef __cplusplus
}
#endif

#endif