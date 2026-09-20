#ifndef DISPENSER_H
#define DISPENSER_H

typedef struct {
    void (*start)(void);
    void (*stop)(void);
    int (*is_active)(void);
} Dispenser;

extern Dispenser simulated_dispenser;

#endif