# Migração ESP32-S3 — Notas de Implementação do Display/Touch

Notas reunidas durante a implementação do driver do display 4,3" 800×480
(RGB + GT911) para o alvo ESP32-S3, usando Arduino IDE (core 3.3.11 = ESP-IDF
5.4). Fonte canônica dos valores: clone de referência do Waveshare (demo
`ESP32_Display_Panel`) e docs do ESP-IDF v5.4.

## 1. Objetivo

Fazer o mesmo projeto LVGL 9.6 (hoje simulador PC) rodar na placa
**SpotPear ESP32-S3-LVGL 4,3"** (mesma base da **Waveshare
ESP32-S3-Touch-LCD-4.3**) via Arduino IDE, sem quebrar o build PC. A UI é
**retrato 480×800**; o painel físico é **800×480** — logo o driver precisa de
**rotação manual** entre o buffer LVGL (retrato) e o frame buffer físico.

## 2. Status atual

- `board_display.cpp` **implementado**: painel RGB 800×480 (ST7262) com
  `esp_lcd_new_rgb_panel`, rotação retrato→paisagem no flush, indev GT911
  por polling I2C (Wire). Retorna o `lv_display_t` real.
- `board_config.h` atualizado com pinos/timings/expansor/touch.
- Falta: **validar o primeiro boot no hardware** (byte-order R/B,
  orientação do touch e da rotação são os knobs ajustáveis).

## 3. Placa e pinos (oficiais, fonte: BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3.h)

Pantalla ST7262 (RGB 16-bit RGB565), touch GT911 (I2C), expansor CH422G (I2C),
backlight via expansor.

### 3.1 LCD RGB (ST7262)

- PCLK = 16 MHz
- Timings: HPW=4, HBP=8, HFP=8, VPW=4, VBP=8, VFP=8
- `pclk_active_neg = 1`
- `data_width = 16`, `bits_per_pixel = 16` (RGB565)
- `bounce_buffer_size_px = 800 * 10` (recomendado p/ evitar drift no S3)
- GPIOs:
  - HSYNC = 46, VSYNC = 3, DE = 5, PCLK = 7, DISP = -1 (não usado)
  - DATA0..15 = {14, 38, 18, 17, 10, 39, 0, 45, 48, 47, 21, 1, 2, 42, 41, 40}
- RST do painel: `LCD_RST = EXIO3` do expansor (não há GPIO direto)

### 3.2 Touch GT911 (I2C)

- Host I2C 0, **SCL=9, SDA=8**, 400 kHz, pullups on
- Endereço 7-bit `0x5D` (alternativo `0x14`)
- `INT = GPIO4`
- RST via expansor: `TP_RST = EXIO1`
- Coordenadas: ler reg `0x814E` (status/count) e pontos a partir de `0x814F`;
  cada ponto = 8 bytes. `x = buf[i*8+3]<<8 | buf[i*8+2]`,
  `y = buf[i*8+5]<<8 | buf[i*8+4]`.
- Limpar buffer: escrever `0x00` em `0x814E`.

### 3.3 Expansor CH422G (I2C, mesmo barramento SCL=9/SDA=8)

Endereços de comando (7-bit) usados pelo driver oficial
(`esp_io_expander_ch422g.c`):

| Registro | Addr 7-bit | Valor de gravação |
|---|---|---|
| WR_SET | 0x24 | bit0 `IO_OE`; bit2 `OD_EN` |
| WR_OC  | 0x23 | 0x0F (default) |
| WR_IO  | 0x38 | 0xFF (todos os IO high) |
| RD_IO  | 0x26 | leitura dos estados |

Config na placa: `EXIO1 = TP_RST`, `EXIO2 = Backlight (on = 1)`,
`EXIO3 = LCD_RST`.

Sequências de reset:

- LCD: `LCD_RST=0` (WR_IO bit3=0) → 10 ms → `LCD_RST=1` → 100 ms.
- Touch: `INT(GPIO4)` output low → 10 ms → `TP_RST=0` (bit1=0) → 100 ms →
  `TP_RST=1` → 200 ms → `gpio_reset_pin(GPIO4)` (volta a input).
- Backlight já liga no início (WR_IO default 0xFF ⇒ EXIO2=1,
  `BACKLIGHT_IDLE_OFF=0`).

Nota: o header da placa define `ESP_PANEL_BOARD_EXPANDER_I2C_ADDRESS (0x20)`,
mas o driver CH422G ignora esse valor e usa os endereços de comando acima.

## 4. API confirmada — ESP-IDF v5.4 (`esp_lcd_rgb_panel_config_t`)

Fonte: docs v5.4 `docs.espressif.com/.../rgb_lcd.html`. **Não** usar
`in_color_format`/`out_color_format` (esses são do v6.1).

```c
esp_lcd_rgb_panel_config_t {
    lcd_clock_source_t clk_src;          // LCD_CLK_SRC_DEFAULT
    esp_lcd_rgb_timing_t timings;        // struct NESTED (resolução + porches)
    size_t data_width;                   // 16
    size_t bits_per_pixel;               // 0 = data_width; usar 16
    size_t num_fbs;                      // 1
    size_t bounce_buffer_size_px;        // 8000
    size_t sram_trans_align;
    size_t psram_trans_align;
    size_t dma_burst_size;
    int hsync_gpio_num;                  // 46
    int vsync_gpio_num;                  // 3
    int de_gpio_num;                     // 5
    int pclk_gpio_num;                   // 7
    int disp_gpio_num;                   // -1 (BL no CH422G!)
    int data_gpio_nums[16];
    // flags:
    uint32_t disp_active_low;
    uint32_t refresh_on_demand;
    uint32_t fb_in_psram;                // 1
    uint32_t double_fb;
    uint32_t no_fb;
    uint32_t bb_invalidate_cache;
};

esp_lcd_rgb_timing_t {
    uint32_t pclk_hz;                    // 16 MHz
    uint32_t h_res;                      // 800
    uint32_t v_res;                      // 480
    uint32_t hsync_pulse_width;          // 4
    uint32_t hsync_back_porch;           // 8
    uint32_t hsync_front_porch;          // 8
    uint32_t vsync_pulse_width;          // 4
    uint32_t vsync_back_porch;           // 8
    uint32_t vsync_front_porch;          // 8
    // flags:
    uint32_t hsync_idle_low;
    uint32_t vsync_idle_low;
    uint32_t de_idle_high;
    uint32_t pclk_active_neg;            // 1
    uint32_t pclk_idle_high;
};
```

Acessos usados: `.timings.flags.pclk_active_neg = 1`,
`.flags.fb_in_psram = 1`, `disp_gpio_num = -1`. Funções: `esp_lcd_new_rgb_panel()`,
`esp_lcd_panel_init()`, `esp_lcd_panel_disp_on_off()` e
`esp_lcd_rgb_panel_get_frame_buffer()`.

## 5. API confirmada — LVGL v9.6 (árvore `lvgl/` do repositório)

- `lv_display_create(hor, ver)` → `lv_display_set_color_format(RGB565)`,
  `lv_display_set_flush_cb`, `lv_display_set_buffers(buf1, buf2, buf_size, render_mode)`,
  `lv_display_set_user_data`, `lv_display_set_default`.
- **`buf_size` é em bytes** (não pixels); render mode FULL exige
  `stride * h <= buf_size`. `LV_DRAW_BUF_ALIGN = 4`, `LV_DRAW_BUF_STRIDE_ALIGN = 1`.
- Flush: `lv_display_flush_ready()`, `lv_display_flush_is_last()`.
- Indev: `lv_indev_create()`, `lv_indev_set_type(LV_INDEV_TYPE_POINTER)`,
  `lv_indev_set_read_cb`, `lv_indev_set_display`, `lv_indev_set_user_data`.
- Tick: `lv_tick_set_cb(cb)` existe (v9.6) — usado no stub atual.
- `config/lv_conf_esp32.h`: já é **RGB565** (`LV_COLOR_FORMAT_DEFAULT`), LVGL 9.6.
  **Não** há `LV_COLOR_DEPTH` (v9 substitui por `LV_COLOR_FORMAT_DEFAULT`).
  `LV_MEM_SIZE = 1 MB` (pool interno). `LV_DRAW_SW_DRAW_UNIT_CNT = 1`.
  Pendência: avaliar `LV_COLOR_16_SWAP` no primeiro boot.

## 6. Ocupação de pinos

Consumidos por LCD/touch:

```
0,1,2,3,4,5,7,8,9,10,14,17,18,21,38,39,40,41,42,45,46,47,48
```

Livres:

```
6,11,12,13,15,16,20,35,36,37
```

`board_config.h` atual conflita (usar estes pinos):

| Periférico | Atual | Conflito | Proposta |
|---|---|---|---|
| Touch SDA/SCL | 38/39 | dados LCD | **8/9** |
| HX711 DATA/SCK | 6/5 | 5 = DE | **6/16** |
| Servo | 17 | DATA3 | **15** |
| Botões | 18/21 | DATA2/DATA10 | **35/36** |
| LED | 2 | DATA12 | **20** |

Proposta final: HX711 6/16, servo 15, botões 35/36, LED 20. Os pinos 35/36 são
somente entrada (ADC) — ok para botões. Validar contra a montagem física.

## 7. Decisões de design do driver (sem libs novas)

- RGB panel via esp_lcd (`fb_in_psram`, bounce buffer 8000 px), pines/timings
  oficiais, `disp_gpio_num = -1`.
- LVGL display **480×800 portait FULL** com buffer em PSRAM + **rotação manual
  no flush**: `px_phys = 799 - ly`, `py_phys = lx`.
- CH422G e GT911 no **mesmo barramento I2C** (SCL=9/SDA=8), via `Wire`.
- Reset sequence: CH422G init → LCD_RST → touch reset (INT/TP_RST).
- Touch inverse: `lx = ny`, `ly = 799 - nx` (com binários/nativas nativas do
  GT911 em 800×480). Primeiro boot é experimental: ajustar rotação/mirror e
  byte order (`LV_COLOR_16_SWAP`) conforme resultado visual.
- `main_esp32.cpp` chama `lv_init()` antes de `board_display_init()`; a UI só
  inicia se `disp != NULL`.

## 8. Build/validação (limitações)

- O agente **não** consegue compilar (`cmake`/`cl`/`gcc` fora do PATH), nem
  executar a GUI, nem ver PNGs.
- Validação de compilação via MSBuild: `build\main.vcxproj` (só PC).
- Arduino IDE: core 3.3.11; **primeiro build baixa os headers IDF**.
- Library Manager não baixa a index → instalar `HX711` (bogde) e `ESP32Servo`
  (madhephaestus) via `Sketch > Include Library > Add .ZIP Library`.
- `real_dispenser.cpp` já usa `#if __has_include(<ESP32Servo.h>)`.
- Headers `esp_lcd_panel_rgb.h` do IDF **não existem localmente** (só após 1º
  build). Raw fetch no GitHub 404 para v5.3/v5.4 → usar docs v5.4 (ok acima).

## 9. Referências usadas

- `$env:TEMP\opencode\ws43` — demo Waveshare (ESP32_Display_Panel +
  esp_lv_adapter). Só referência; **não** seguir adição de libs.
- `.../libraries/ESP32_Display_Panel/src/board/supported/waveshare/BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3.h`
- `.../libraries/ESP32_IO_Expander/src/port/esp_io_expander_ch422g.c`
- `.../libraries/ESP32_Display_Panel/src/drivers/touch/port/esp_lcd_touch_gt911.c`
- `docs.espressif.com/.../v5.4/.../rgb_lcd.html`

## 10. Próximos passos

1. Implementar `board_display.cpp` (chave da migração).
2. Atualizar `board_config.h` (touch 8/9 + LCD/timings/CH422G + remap).
3. Sincronizar `arduino/README.md` (`## Estado`) e `.docs` afetados.
4. Usuário: instalar libs, compilar na Arduino IDE, gravar e reportar 1º boot.