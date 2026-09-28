#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "esp_heap_caps.h"

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

void lv_mem_init(void)
{
    return;
}

void lv_mem_deinit(void)
{
    return;
}

lv_mem_pool_t lv_mem_add_pool(void * mem, size_t bytes)
{
    (void)mem;
    (void)bytes;
    return NULL;
}

void lv_mem_remove_pool(lv_mem_pool_t pool)
{
    (void)pool;
    return;
}

void * lv_malloc_core(size_t size)
{
    void * p = heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (p == NULL) {
        p = heap_caps_malloc(size, MALLOC_CAP_8BIT);
    }
    return p;
}

void * lv_realloc_core(void * p, size_t new_size)
{
    if (p == NULL) {
        return lv_malloc_core(new_size);
    }
    void * np = heap_caps_realloc(p, new_size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (np == NULL) {
        np = heap_caps_realloc(p, new_size, MALLOC_CAP_8BIT);
    }
    return np;
}

void lv_free_core(void * p)
{
    if (p != NULL) {
        heap_caps_free(p);
    }
}

void lv_mem_monitor_core(lv_mem_monitor_t * mon_p)
{
    (void)mon_p;
    return;
}

lv_result_t lv_mem_test_core(void)
{
    return LV_RESULT_OK;
}

#ifdef __cplusplus
}
#endif