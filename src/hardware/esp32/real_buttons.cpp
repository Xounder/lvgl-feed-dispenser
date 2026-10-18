#include "real_buttons.h"

#include <Arduino.h>

#include "../button.h"
#include "board_config.h"

typedef struct {
    int pin;
    int stable;
    int raw;
    uint32_t last_change_ms;
    const char *name;
} DebouncedButton;

static DebouncedButton emergency_btn = {
    BOARD_BUTTON_2_PIN, 0, 0, 0, "EMERGENCIA"
};

static DebouncedButton manual_btn = {
    BOARD_BUTTON_1_PIN, 0, 0, 0, "LIBERACAO"
};

static int read_raw(int pin)
{
    int level = digitalRead(pin);

    return level == BOARD_BUTTON_ACTIVE_LEVEL ? 1 : 0;
}

static int debounce_update(DebouncedButton *button)
{
    int value = read_raw(button->pin);
    uint32_t now = millis();

    if (value != button->raw) {
        button->raw = value;
        button->last_change_ms = now;
    }

    if ((now - button->last_change_ms) >= BOARD_BUTTON_DEBOUNCE_MS &&
        button->stable != button->raw) {
        button->stable = button->raw;

        Serial.printf(
            "[BOTAO] %s GPIO%d %s\n",
            button->name,
            button->pin,
            button->stable ? "PRESSIONADO" : "SOLTO"
        );
    }

    return button->stable;
}

static int emergency_is_pressed(void)
{
    return debounce_update(&emergency_btn);
}

static int manual_release_is_pressed(void)
{
    return debounce_update(&manual_btn);
}

Button emergency_button = {
    emergency_is_pressed
};

Button manual_release_button = {
    manual_release_is_pressed
};

void real_buttons_init(void)
{
    pinMode(BOARD_BUTTON_1_PIN, INPUT_PULLUP);
    pinMode(BOARD_BUTTON_2_PIN, INPUT_PULLUP);

    emergency_btn.raw = read_raw(emergency_btn.pin);
    emergency_btn.stable = emergency_btn.raw;
    emergency_btn.last_change_ms = millis();

    manual_btn.raw = read_raw(manual_btn.pin);
    manual_btn.stable = manual_btn.raw;
    manual_btn.last_change_ms = millis();

    Serial.printf(
        "[BOTAO] init LIBERACAO=GPIO%d EMERGENCIA=GPIO%d ativo=%d "
        "liberacao=%d emergencia=%d\n",
        BOARD_BUTTON_1_PIN,
        BOARD_BUTTON_2_PIN,
        BOARD_BUTTON_ACTIVE_LEVEL,
        manual_btn.stable,
        emergency_btn.stable
    );
}
