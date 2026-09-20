#ifndef WEIGHT_SENSOR_H
#define WEIGHT_SENSOR_H

typedef struct {
    int (*read_grams)(void);
    void (*add_grams)(int grams);
    void (*reset)(void);
} WeightSensor;

extern WeightSensor simulated_weight_sensor;

#endif