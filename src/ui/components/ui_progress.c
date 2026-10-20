#include "ui_progress.h"
#include "screen_chrome.h"

void ui_progress_create(
    lv_obj_t *parent,
    lv_coord_t y,
    lv_color_t indicator_color,
    UiProgress *out
)
{
    lv_obj_t *row = lv_obj_create(parent);
    lv_obj_set_size(row, 440, 26);
    lv_obj_align(row, LV_ALIGN_TOP_MID, 0, y);
    lv_obj_clear_flag(row, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(row, 0, 0);
    lv_obj_set_style_bg_opa(row, LV_OPA_TRANSP, 0);

    out->bar = lv_bar_create(row);
    lv_obj_set_size(out->bar, 380, 18);
    lv_obj_align(out->bar, LV_ALIGN_LEFT_MID, 0, 0);
    lv_bar_set_range(out->bar, 0, 100);
    lv_bar_set_value(out->bar, 0, LV_ANIM_OFF);
    lv_obj_set_style_bg_color(out->bar, CHROME_BORDER, LV_PART_MAIN);
    lv_obj_set_style_bg_color(out->bar, indicator_color, LV_PART_INDICATOR);

    out->percent = lv_label_create(row);
    lv_label_set_text(out->percent, "0%");
    lv_obj_set_style_text_color(out->percent, CHROME_WHITE, 0);
    lv_obj_set_style_text_font(out->percent, &lv_font_montserrat_14, 0);
    lv_obj_align(out->percent, LV_ALIGN_RIGHT_MID, 0, 0);
}
