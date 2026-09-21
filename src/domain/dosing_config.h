#ifndef DOSING_CONFIG_H
#define DOSING_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DOSING_MODE_GRAMS,
    DOSING_MODE_CURRENCY
} DosingMode;

typedef struct {
    DosingMode mode;
    int target_grams;          /* modo massa (g) */
    int target_money_cents;    /* modo valor (R$ em centavos) */
    int price_per_kg_cents;    /* preco de referencia por kg (centavos) */
} DosingConfig;

#ifdef __cplusplus
}
#endif

#endif