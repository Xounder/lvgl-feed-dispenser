#ifndef BUTTON_H
#define BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int (*is_pressed)(void);
} Button;

extern Button emergency_button;
extern Button manual_release_button;

#ifdef __cplusplus
}
#endif

#endif
