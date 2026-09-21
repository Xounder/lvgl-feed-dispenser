#ifndef WEIGHT_SENSOR_H
#define WEIGHT_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int (*read_grams)(void);
    void (*add_grams)(int grams);
    void (*reset)(void);
} WeightSensor;

extern WeightSensor weight_sensor;

#ifdef __cplusplus
}
#endif

#endif