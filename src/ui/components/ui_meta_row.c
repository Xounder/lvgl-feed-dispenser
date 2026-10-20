#include "ui_meta_row.h"
#include "screen_chrome.h"

void ui_meta_row_create(
    lv_obj_t *parent,
    const char *label,
    lv_coord_t y,
    UiMetaRow *out
)
{
    lv_obj_t *row = lv_obj_create(parent);
    lv_obj_set_size(row, 440, 24);
    lv_obj_align(row, LV_ALIGN_TOP_MID, 0, y);
    lv_obj_clear_flag(row, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(row, 0, 0);
    lv_obj_set_style_bg_opa(row, LV_OPA_TRANSP, 0);

    lv_obj_t *label_obj = lv_label_create(row);
    lv_label_set_text(label_obj, label);
    lv_obj_set_style_text_color(label_obj, CHROME_GREY, 0);
    lv_obj_set_style_text_font(label_obj, &lv_font_montserrat_14, 0);
    lv_obj_align(label_obj, LV_ALIGN_LEFT_MID, 0, 0);

    out->value = lv_label_create(row);
    lv_obj_set_style_text_color(out->value, CHROME_GREY, 0);
    lv_obj_set_style_text_font(out->value, &lv_font_montserrat_14, 0);
    lv_obj_align(out->value, LV_ALIGN_RIGHT_MID, 0, 0);
}
