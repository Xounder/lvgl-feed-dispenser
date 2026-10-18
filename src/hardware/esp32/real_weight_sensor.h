#ifndef REAL_WEIGHT_SENSOR_H
#define REAL_WEIGHT_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

void real_weight_sensor_init(void);

void real_weight_sensor_probe(const char *tag);

void real_weight_sensor_log(void);

#ifdef __cplusplus
}
#endif

#endif