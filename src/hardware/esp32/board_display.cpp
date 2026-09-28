#include "board_display.h"

#include <Arduino.h>
#include <Wire.h>
#include <stddef.h>
#include <stdint.h>

#include "driver/gpio.h"
#include "esp_heap_caps.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_timer.h"

#include "board_config.h"

static esp_lcd_panel_handle_t s_panel = NULL;
static void *s_fb = NULL;
static uint8_t s_touch_addr = BOARD_TOUCH_I2C_ADDR;

static uint32_t board_tick_ms(void)
{
    return (uint32_t)(esp_timer_get_time() / 1000);
}

static bool touch_reg_read(uint16_t reg, uint8_t *data, size_t len)
{
    Wire.beginTransmission(s_touch_addr);
    Wire.write((uint8_t)(reg >> 8));
    Wire.write((uint8_t)(reg & 0xFF));
    if (Wire.endTransmission(false) != 0) {
        return false;
    }
    if (Wire.requestFrom((int)s_touch_addr, (int)len) != (int)len) {
        return false;
    }
    for (size_t i = 0; i < len; i++) {
        data[i] = Wire.read();
    }
    return true;
}

static void touch_reg_write(uint16_t reg, uint8_t value)
{
    Wire.beginTransmission(s_touch_addr);
    Wire.write((uint8_t)(reg >> 8));
    Wire.write((uint8_t)(reg & 0xFF));
    Wire.write(value);
    Wire.endTransmission();
}

static void flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    uint16_t *src = (uint16_t *)px_map;
    uint16_t *dst = (uint16_t *)s_fb;
    for (int ly = area->y1; ly <= area->y2; ly++) {
        for (int lx = area->x1; lx <= area->x2; lx++) {
            int px = (BOARD_DISPLAY_PANEL_HOR_RES - 1) - ly;
            int py = lx;
            dst[(size_t)py * BOARD_DISPLAY_PANEL_HOR_RES + px] = src[(size_t)ly * BOARD_DISPLAY_HOR_RES + lx];
        }
    }
    lv_display_flush_ready(disp);
}

static void touch_read_cb(lv_indev_t *indev, lv_indev_data_t *data)
{
    data->state = LV_INDEV_STATE_RELEASED;
    uint8_t status = 0;
    if (touch_reg_read(BOARD_TOUCH_STATUS_REG, &status, 1) && (status & 0x80) != 0) {
        uint8_t count = status & 0x0F;
        if (count > 0) {
            uint8_t buf[8] = {0};
            if (touch_reg_read(BOARD_TOUCH_STATUS_REG + 1, buf, 8)) {
                uint16_t nx = (uint16_t)((buf[2] << 8) | buf[1]);
                uint16_t ny = (uint16_t)((buf[4] << 8) | buf[3]);
                data->point.x = ny;
                data->point.y = (BOARD_DISPLAY_PANEL_HOR_RES - 1) - nx;
                data->state = LV_INDEV_STATE_PRESSED;
            }
            touch_reg_write(BOARD_TOUCH_STATUS_REG, 0x00);
            return;
        }
    }
    touch_reg_write(BOARD_TOUCH_STATUS_REG, 0x00);
}

lv_display_t *board_display_init(void)
{
    lv_tick_set_cb(board_tick_ms);

    gpio_set_direction((gpio_num_t)BOARD_TOUCH_RST_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level((gpio_num_t)BOARD_TOUCH_RST_PIN, BOARD_TOUCH_RST_ACTIVE_LEVEL);
    delay(20);
    gpio_set_level((gpio_num_t)BOARD_TOUCH_RST_PIN, !BOARD_TOUCH_RST_ACTIVE_LEVEL);
    delay(100);

    Wire.begin(BOARD_TOUCH_I2C_SDA_PIN, BOARD_TOUCH_I2C_SCL_PIN, BOARD_TOUCH_I2C_FREQ);
    gpio_set_pull_mode((gpio_num_t)BOARD_TOUCH_I2C_SDA_PIN, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode((gpio_num_t)BOARD_TOUCH_I2C_SCL_PIN, GPIO_PULLUP_ONLY);

    static const uint8_t dbg_addrs[] = {0x5D, 0x14};
    s_touch_addr = 0;
    for (size_t i = 0; i < sizeof(dbg_addrs) / sizeof(dbg_addrs[0]); i++) {
        Wire.beginTransmission(dbg_addrs[i]);
        if (Wire.endTransmission() == 0) {
            s_touch_addr = dbg_addrs[i];
            break;
        }
    }
    if (s_touch_addr == 0) {
        s_touch_addr = BOARD_TOUCH_I2C_ADDR;
    }

    gpio_set_direction((gpio_num_t)BOARD_BACKLIGHT_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level((gpio_num_t)BOARD_BACKLIGHT_PIN, BOARD_BACKLIGHT_ACTIVE_LEVEL);

    esp_lcd_rgb_panel_config_t panel_cfg = {};
    panel_cfg.clk_src = LCD_CLK_SRC_DEFAULT;
    panel_cfg.timings.pclk_hz = BOARD_LCD_PCLK_HZ;
    panel_cfg.timings.h_res = BOARD_DISPLAY_PANEL_HOR_RES;
    panel_cfg.timings.v_res = BOARD_DISPLAY_PANEL_VER_RES;
    panel_cfg.timings.hsync_pulse_width = BOARD_LCD_HSYNC_PULSE_WIDTH;
    panel_cfg.timings.hsync_back_porch = BOARD_LCD_HSYNC_BACK_PORCH;
    panel_cfg.timings.hsync_front_porch = BOARD_LCD_HSYNC_FRONT_PORCH;
    panel_cfg.timings.vsync_pulse_width = BOARD_LCD_VSYNC_PULSE_WIDTH;
    panel_cfg.timings.vsync_back_porch = BOARD_LCD_VSYNC_BACK_PORCH;
    panel_cfg.timings.vsync_front_porch = BOARD_LCD_VSYNC_FRONT_PORCH;
    panel_cfg.timings.flags.pclk_active_neg = BOARD_LCD_PCLK_ACTIVE_NEG;
    panel_cfg.data_width = 16;
    panel_cfg.bits_per_pixel = 16;
    panel_cfg.num_fbs = 1;
    panel_cfg.bounce_buffer_size_px = BOARD_LCD_BOUNCE_BUFFER_SIZE_PX;
    panel_cfg.dma_burst_size = 64;
    panel_cfg.hsync_gpio_num = BOARD_LCD_HSYNC_PIN;
    panel_cfg.vsync_gpio_num = BOARD_LCD_VSYNC_PIN;
    panel_cfg.de_gpio_num = BOARD_LCD_DE_PIN;
    panel_cfg.pclk_gpio_num = BOARD_LCD_PCLK_PIN;
    panel_cfg.disp_gpio_num = BOARD_LCD_DISP_PIN;
    const int data_pins[] = BOARD_LCD_DATA_PINS;
    for (size_t i = 0; i < SOC_LCDCAM_RGB_DATA_WIDTH; i++) {
        panel_cfg.data_gpio_nums[i] = (i < sizeof(data_pins) / sizeof(data_pins[0])) ? data_pins[i] : -1;
    }
    panel_cfg.flags.fb_in_psram = 1;

    esp_err_t err = esp_lcd_new_rgb_panel(&panel_cfg, &s_panel);
    if (err != ESP_OK) {
        Serial.printf("LCD1 new_rgb_panel falhou: %s\n", esp_err_to_name(err));
        return NULL;
    }
    err = esp_lcd_panel_init(s_panel);
    if (err != ESP_OK) {
        Serial.printf("LCD2 panel_init falhou: %s\n", esp_err_to_name(err));
        return NULL;
    }
    err = esp_lcd_panel_disp_on_off(s_panel, true);
    if (err != ESP_OK) {
        Serial.printf("LCD3 disp_on_off (nao fatal): %s\n", esp_err_to_name(err));
    }
    err = esp_lcd_rgb_panel_get_frame_buffer(s_panel, 1, &s_fb);
    if (err != ESP_OK) {
        Serial.printf("LCD4 get_frame_buffer falhou: %s\n", esp_err_to_name(err));
        return NULL;
    }

    lv_display_t *disp = lv_display_create(BOARD_DISPLAY_HOR_RES, BOARD_DISPLAY_VER_RES);
    if (disp == NULL) {
        Serial.println("LCD5 lv_display_create retornou NULL");
        return NULL;
    }
    lv_display_set_flush_cb(disp, flush_cb);
    lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565);

    size_t buf_size = (size_t)BOARD_DISPLAY_HOR_RES * BOARD_DISPLAY_VER_RES * 2;
    void *buf = heap_caps_malloc(buf_size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (buf == NULL) {
        Serial.println("LCD6 heap_caps_malloc SPIRAM falhou");
        return NULL;
    }
    lv_display_set_buffers_with_stride(disp, buf, NULL, buf_size, BOARD_DISPLAY_HOR_RES * 2, LV_DISPLAY_RENDER_MODE_FULL);

    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, touch_read_cb);
    lv_indev_set_display(indev, disp);

    return disp;
}