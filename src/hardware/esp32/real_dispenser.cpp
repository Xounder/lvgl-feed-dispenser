#include "real_dispenser.h"

#include <Arduino.h>
#if __has_include(<ESP32Servo.h>)
#include <ESP32Servo.h>
#else
#include <Servo.h>
#endif

#include "../dispenser.h"
#include "board_config.h"

static Servo servo;
static int active = 0;

static void real_start(void)
{
    servo.write(BOARD_SERVO_OPEN_ANGLE);
    active = 1;
}

static void real_stop(void)
{
    servo.write(BOARD_SERVO_CLOSED_ANGLE);
    active = 0;
}

static int real_is_active(void)
{
    return active;
}

Dispenser dispenser = {
    real_start,
    real_stop,
    real_is_active
};

void real_dispenser_init(void)
{
    servo.attach(BOARD_SERVO_PIN);
    servo.write(BOARD_SERVO_CLOSED_ANGLE);
}