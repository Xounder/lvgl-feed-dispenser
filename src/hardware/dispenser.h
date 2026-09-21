#ifndef DISPENSER_H
#define DISPENSER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void (*start)(void);
    void (*stop)(void);
    int (*is_active)(void);
} Dispenser;

extern Dispenser dispenser;

#ifdef __cplusplus
}
#endif

#endif