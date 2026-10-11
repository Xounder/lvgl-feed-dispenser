#ifndef LINK_UART_H
#define LINK_UART_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void (*begin)(void);
    int (*write)(const uint8_t *data, int len);
    int (*read)(uint8_t *byte);
} LinkUart;

extern LinkUart link_uart;

#ifdef __cplusplus
}
#endif

#endif
