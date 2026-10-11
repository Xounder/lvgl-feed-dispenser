#ifndef LINK_PROXY_H
#define LINK_PROXY_H

#ifdef __cplusplus
extern "C" {
#endif

void link_proxy_poll(void);
int link_proxy_is_up(void);
int link_proxy_error(void);

#ifdef __cplusplus
}
#endif

#endif
