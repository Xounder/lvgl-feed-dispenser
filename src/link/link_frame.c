#include "link_frame.h"

enum {
    LINK_PARSE_HEADER,
    LINK_PARSE_LEN,
    LINK_PARSE_MSG,
    LINK_PARSE_PAYLOAD,
    LINK_PARSE_CRC
};

static uint8_t crc8_step(uint8_t crc, uint8_t byte)
{
    int i;

    crc ^= byte;

    for (i = 0; i < 8; i++) {
        if (crc & 0x80) {
            crc = (uint8_t)((crc << 1) ^ 0x07);
        } else {
            crc = (uint8_t)(crc << 1);
        }
    }

    return crc;
}

uint8_t link_crc8(const uint8_t *data, int len)
{
    uint8_t crc = 0;
    int i;

    for (i = 0; i < len; i++) {
        crc = crc8_step(crc, data[i]);
    }

    return crc;
}

int link_frame_encode(
    uint8_t msg_id,
    const uint8_t *payload,
    uint8_t len,
    uint8_t *out,
    int out_cap
)
{
    int i;

    if (len > LINK_MAX_PAYLOAD) {
        return 0;
    }

    if (out_cap < (int)len + 4) {
        return 0;
    }

    out[0] = LINK_FRAME_HEADER;
    out[1] = len;
    out[2] = msg_id;

    for (i = 0; i < len; i++) {
        out[3 + i] = payload[i];
    }

    out[3 + len] = crc8_step(crc8_step(0, len), msg_id);

    for (i = 0; i < len; i++) {
        out[3 + len] = crc8_step(out[3 + len], payload[i]);
    }

    return (int)len + 4;
}

void link_parser_init(LinkParser *parser)
{
    parser->state = LINK_PARSE_HEADER;
    parser->msg_id = 0;
    parser->len = 0;
    parser->index = 0;
    parser->crc = 0;
}

int link_parser_put(LinkParser *parser, uint8_t byte, LinkFrame *frame)
{
    switch (parser->state) {
        case LINK_PARSE_HEADER:
            if (byte == LINK_FRAME_HEADER) {
                parser->state = LINK_PARSE_LEN;
            }
            break;

        case LINK_PARSE_LEN:
            if (byte > LINK_MAX_PAYLOAD) {
                parser->state = LINK_PARSE_HEADER;
                break;
            }
            parser->len = byte;
            parser->crc = crc8_step(0, byte);
            parser->state = LINK_PARSE_MSG;
            break;

        case LINK_PARSE_MSG:
            parser->msg_id = byte;
            parser->crc = crc8_step(parser->crc, byte);
            parser->index = 0;
            parser->state = (parser->len > 0)
                ? LINK_PARSE_PAYLOAD
                : LINK_PARSE_CRC;
            break;

        case LINK_PARSE_PAYLOAD:
            parser->payload[parser->index] = byte;
            parser->crc = crc8_step(parser->crc, byte);
            parser->index++;
            if (parser->index >= parser->len) {
                parser->state = LINK_PARSE_CRC;
            }
            break;

        case LINK_PARSE_CRC: {
            int ok = (byte == parser->crc);

            parser->state = LINK_PARSE_HEADER;

            if (ok) {
                int i;

                frame->msg_id = parser->msg_id;
                frame->len = parser->len;

                for (i = 0; i < parser->len; i++) {
                    frame->payload[i] = parser->payload[i];
                }

                return 1;
            }
            break;
        }

        default:
            parser->state = LINK_PARSE_HEADER;
            break;
    }

    return 0;
}
