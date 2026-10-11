#ifndef INDICATOR_H
#define INDICATOR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void (*set)(int on);
} Indicator;

extern Indicator indicator;

#ifdef __cplusplus
}
#endif

#endif
