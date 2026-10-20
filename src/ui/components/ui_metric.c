#include "ui_metric.h"
#include "screen_chrome.h"

void ui_metric_create(
    lv_obj_t *parent,
    const char *hint,
    lv_coord_t hint_y,
    lv_coord_t value_y,
    lv_coord_t value_x,
    UiMetric *out
)
{
    out->hint = lv_label_create(parent);
    lv_label_set_text(out->hint, hint);
    lv_obj_set_style_text_color(out->hint, CHROME_GREY, 0);
    lv_obj_set_style_text_font(out->hint, &lv_font_montserrat_14, 0);
    lv_obj_align(out->hint, LV_ALIGN_TOP_MID, 0, hint_y);

    out->value = lv_label_create(parent);
    lv_label_set_text(out->value, "0 g");
    lv_obj_set_style_text_color(out->value, CHROME_WHITE, 0);
    lv_obj_set_style_text_font(out->value, &lv_font_montserrat_28, 0);
    lv_obj_align(out->value, LV_ALIGN_TOP_MID, value_x, value_y);
}
