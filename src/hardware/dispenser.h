#ifndef DISPENSER_H
#define DISPENSER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DISPENSER_FLOW_FAST,
    DISPENSER_FLOW_FINE,
    DISPENSER_FLOW_MANUAL
} DispenserFlow;

typedef struct {
    void (*start)(DispenserFlow flow);
    void (*stop)(void);
    int (*is_active)(void);
} Dispenser;

extern Dispenser dispenser;

#ifdef __cplusplus
}
#endif

#endif
