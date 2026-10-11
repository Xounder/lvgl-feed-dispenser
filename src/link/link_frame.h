#ifndef LINK_FRAME_H
#define LINK_FRAME_H

#include <stdint.h>

#include "link_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t msg_id;
    uint8_t len;
    uint8_t payload[LINK_MAX_PAYLOAD];
} LinkFrame;

typedef struct {
    uint8_t state;
    uint8_t msg_id;
    uint8_t len;
    uint8_t index;
    uint8_t payload[LINK_MAX_PAYLOAD];
    uint8_t crc;
} LinkParser;

uint8_t link_crc8(const uint8_t *data, int len);

int link_frame_encode(
    uint8_t msg_id,
    const uint8_t *payload,
    uint8_t len,
    uint8_t *out,
    int out_cap
);

void link_parser_init(LinkParser *parser);

int link_parser_put(LinkParser *parser, uint8_t byte, LinkFrame *frame);

#ifdef __cplusplus
}
#endif

#endif
