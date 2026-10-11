#ifdef ROLE_EXTERNAL

#include <Arduino.h>
#include <Preferences.h>

#include "link_backend.h"
#include "link_config.h"
#include "link_frame.h"
#include "link_uart.h"
#include "../domain/dosing_controller.h"
#include "../hardware/esp32/real_weight_sensor.h"

static DosingConfig backend_config = {
    DOSING_MODE_GRAMS,
    100,
    500,
    1200
};

static LinkParser parser;
static unsigned long last_tx_ms = 0;
static Preferences prefs;

static void config_save(void)
{
    prefs.begin("dosing", false);
    prefs.putUChar("mode", (uint8_t)backend_config.mode);
    prefs.putUShort("grams", (uint16_t)backend_config.target_grams);
    prefs.putUShort("money", (uint16_t)backend_config.target_money_cents);
    prefs.putUShort("price", (uint16_t)backend_config.price_per_kg_cents);
    prefs.end();
}

static void config_load(void)
{
    prefs.begin("dosing", true);

    if (prefs.isKey("grams")) {
        backend_config.mode =
            (DosingMode)prefs.getUChar("mode", (uint8_t)backend_config.mode);
        backend_config.target_grams =
            prefs.getUShort("grams", (uint16_t)backend_config.target_grams);
        backend_config.target_money_cents =
            prefs.getUShort("money", (uint16_t)backend_config.target_money_cents);
        backend_config.price_per_kg_cents =
            prefs.getUShort("price", (uint16_t)backend_config.price_per_kg_cents);
    }

    prefs.end();
}

static void send_msg(uint8_t msg_id, const uint8_t *payload, uint8_t len)
{
    uint8_t buffer[LINK_MAX_PAYLOAD + 4];
    int n = link_frame_encode(msg_id, payload, len, buffer, sizeof(buffer));

    if (n > 0) {
        link_uart.write(buffer, n);
    }
}

static void send_ack(void)
{
    send_msg(LINK_MSG_ACK, NULL, 0);
}

static int read_le16(const uint8_t *data)
{
    return data[0] | (data[1] << 8);
}

static void send_value_u16(uint8_t msg_id, int value)
{
    uint8_t payload[2];

    payload[0] = (uint8_t)(value & 0xff);
    payload[1] = (uint8_t)((value >> 8) & 0xff);

    send_msg(msg_id, payload, 2);
}

static void handle_config_set(const LinkFrame *frame)
{
    if (frame->len < 7) {
        return;
    }

    backend_config.mode = (DosingMode)frame->payload[0];
    backend_config.target_grams = read_le16(&frame->payload[1]);
    backend_config.target_money_cents = read_le16(&frame->payload[3]);
    backend_config.price_per_kg_cents = read_le16(&frame->payload[5]);

    config_save();
}

static void handle_command(const LinkFrame *frame)
{
    switch (frame->msg_id) {
        case LINK_MSG_START:
            dosing_controller_start();
            send_ack();
            break;

        case LINK_MSG_CANCEL:
            dosing_controller_cancel();
            send_ack();
            break;

        case LINK_MSG_NEW_DOSING:
            dosing_controller_new_dosing();
            send_ack();
            break;

        case LINK_MSG_MANUAL_RELEASE_START:
            dosing_controller_manual_release_start();
            send_ack();
            break;

        case LINK_MSG_MANUAL_RELEASE_STOP:
            dosing_controller_manual_release_stop();
            send_ack();
            break;

        case LINK_MSG_CONFIG_SET:
            handle_config_set(frame);
            send_ack();
            break;

        case LINK_MSG_PING:
            send_msg(LINK_MSG_PONG, NULL, 0);
            break;

        default:
            break;
    }
}

void link_backend_init(void)
{
    link_parser_init(&parser);

    link_uart.begin();

    config_load();

    dosing_controller_init(&backend_config);

    last_tx_ms = millis();
}

void link_backend_send_error(uint8_t code)
{
    send_msg(LINK_MSG_ERROR, &code, 1);
}

void link_backend_poll(void)
{
    uint8_t byte;
    LinkFrame frame;

    while (link_uart.read(&byte)) {
        if (link_parser_put(&parser, byte, &frame)) {
            handle_command(&frame);
        }
    }
}

void link_backend_tick(void)
{
    uint8_t payload[2];

    dosing_controller_update();

    if ((millis() - last_tx_ms) < LINK_TELEMETRY_MS) {
        return;
    }

    last_tx_ms = millis();

    payload[0] = (uint8_t)dosing_controller_get_state();
    payload[1] = (uint8_t)dosing_controller_get_phase();
    send_msg(LINK_MSG_STATE, payload, 2);

    send_value_u16(LINK_MSG_WEIGHT, dosing_controller_get_weight());
    send_value_u16(LINK_MSG_TARGET, dosing_controller_get_target_grams());

    payload[0] = dosing_controller_manual_release_is_active() ? 1 : 0;
    send_msg(LINK_MSG_MANUAL_RELEASE_ACTIVE, payload, 1);

    if (!real_weight_sensor_is_ready()) {
        link_backend_send_error(LINK_ERROR_HX711);
    }
}

#endif
