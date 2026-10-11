#ifndef LINK_BACKEND_H
#define LINK_BACKEND_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void link_backend_init(void);
void link_backend_poll(void);
void link_backend_tick(void);
void link_backend_send_error(uint8_t code);

#ifdef __cplusplus
}
#endif

#endif
