#ifdef ROLE_SCREEN

#include <Arduino.h>

#include "link_proxy.h"
#include "link_config.h"
#include "link_frame.h"
#include "link_uart.h"
#include "../domain/dosing_controller.h"

static DosingConfig *proxy_config = NULL;
static DosingState mirror_state = DOSING_STATE_IDLE;
static DosingPhase mirror_phase = DOSING_PHASE_FAST;
static int mirror_weight = 0;
static int mirror_target = 0;
static int mirror_manual_active = 0;
static uint8_t mirror_error = LINK_ERROR_NONE;
static unsigned long last_rx_ms = 0;
static unsigned long last_retry_ms = 0;
static LinkParser parser;

static void send_msg(uint8_t msg_id, const uint8_t *payload, uint8_t len)
{
    uint8_t buffer[LINK_MAX_PAYLOAD + 4];
    int n = link_frame_encode(msg_id, payload, len, buffer, sizeof(buffer));

    if (n > 0) {
        link_uart.write(buffer, n);
    }
}

static void put_le16(uint8_t *out, int value)
{
    out[0] = (uint8_t)(value & 0xff);
    out[1] = (uint8_t)((value >> 8) & 0xff);
}

static int read_le16(const uint8_t *data)
{
    return data[0] | (data[1] << 8);
}

static int local_target_grams(void)
{
    if (proxy_config == NULL) {
        return mirror_target;
    }

    if (proxy_config->mode == DOSING_MODE_GRAMS) {
        return proxy_config->target_grams;
    }

    if (proxy_config->price_per_kg_cents <= 0) {
        return 0;
    }

    return (proxy_config->target_money_cents * 1000) /
           proxy_config->price_per_kg_cents;
}

static void send_config(void)
{
    uint8_t payload[7];

    if (proxy_config == NULL) {
        return;
    }

    payload[0] = (uint8_t)proxy_config->mode;
    put_le16(&payload[1], proxy_config->target_grams);
    put_le16(&payload[3], proxy_config->target_money_cents);
    put_le16(&payload[5], proxy_config->price_per_kg_cents);

    send_msg(LINK_MSG_CONFIG_SET, payload, 7);
}

static void apply_telemetry(const LinkFrame *frame)
{
    switch (frame->msg_id) {
        case LINK_MSG_STATE:
            if (frame->len >= 2) {
                mirror_state = (DosingState)frame->payload[0];
                mirror_phase = (DosingPhase)frame->payload[1];
            }
            break;

        case LINK_MSG_WEIGHT:
            if (frame->len >= 2) {
                mirror_weight = read_le16(frame->payload);
            }
            break;

        case LINK_MSG_TARGET:
            if (frame->len >= 2) {
                mirror_target = read_le16(frame->payload);
            }
            break;

        case LINK_MSG_MANUAL_RELEASE_ACTIVE:
            if (frame->len >= 1) {
                mirror_manual_active = frame->payload[0];
            }
            break;

        case LINK_MSG_ERROR:
            if (frame->len >= 1) {
                mirror_error = frame->payload[0];
            }
            break;

        default:
            break;
    }
}

void link_proxy_poll(void)
{
    uint8_t byte;
    LinkFrame frame;

    if (proxy_config == NULL) {
        return;
    }

    while (link_uart.read(&byte)) {
        if (link_parser_put(&parser, byte, &frame)) {
            apply_telemetry(&frame);
            last_rx_ms = millis();
        }
    }

    if (!link_proxy_is_up() && (millis() - last_retry_ms) >= LINK_RETRY_MS) {
        last_retry_ms = millis();
        send_config();
        send_msg(LINK_MSG_PING, NULL, 0);
    }
}

int link_proxy_is_up(void)
{
    if (proxy_config == NULL) {
        return 0;
    }

    return (millis() - last_rx_ms) < LINK_TIMEOUT_MS;
}

int link_proxy_error(void)
{
    return (int)mirror_error;
}

void dosing_controller_init(DosingConfig *config)
{
    proxy_config = config;
    mirror_state = DOSING_STATE_IDLE;
    mirror_phase = DOSING_PHASE_FAST;
    mirror_weight = 0;
    mirror_target = 0;
    mirror_manual_active = 0;
    mirror_error = LINK_ERROR_NONE;

    link_parser_init(&parser);
    link_uart.begin();

    last_rx_ms = millis();
    last_retry_ms = millis();

    send_config();
    send_msg(LINK_MSG_PING, NULL, 0);
}

void dosing_controller_start(void)
{
    if (!link_proxy_is_up()) {
        return;
    }

    send_config();
    send_msg(LINK_MSG_START, NULL, 0);
}

void dosing_controller_update(void)
{
    link_proxy_poll();
}

void dosing_controller_cancel(void)
{
    send_msg(LINK_MSG_CANCEL, NULL, 0);
}

void dosing_controller_new_dosing(void)
{
    if (!link_proxy_is_up()) {
        return;
    }

    send_msg(LINK_MSG_NEW_DOSING, NULL, 0);
}

void dosing_controller_manual_release_start(void)
{
    if (!link_proxy_is_up()) {
        return;
    }

    if (!dosing_controller_manual_release_is_allowed()) {
        return;
    }

    send_msg(LINK_MSG_MANUAL_RELEASE_START, NULL, 0);
}

void dosing_controller_manual_release_stop(void)
{
    send_msg(LINK_MSG_MANUAL_RELEASE_STOP, NULL, 0);
}

int dosing_controller_manual_release_is_active(void)
{
    return mirror_manual_active;
}

int dosing_controller_manual_release_is_allowed(void)
{
    return mirror_state != DOSING_STATE_DOSING;
}

int dosing_controller_get_weight(void)
{
    return mirror_weight;
}

int dosing_controller_get_target_grams(void)
{
    return local_target_grams();
}

DosingState dosing_controller_get_state(void)
{
    return mirror_state;
}

DosingPhase dosing_controller_get_phase(void)
{
    return mirror_phase;
}

#endif
