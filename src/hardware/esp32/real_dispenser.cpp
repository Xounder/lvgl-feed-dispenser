#include "real_dispenser.h"

#include <Arduino.h>
#include <ESP32Servo.h>

#include "../dispenser.h"
#include "board_config.h"

static Servo servo;
static int active = 0;

static void servo_write(int angle, const char *action)
{
    servo.write(angle);

    Serial.printf(
        "[SERVO] %s angulo=%d graus pulso=%dus (pino=%d)\n",
        action,
        angle,
        servo.readMicroseconds(),
        BOARD_SERVO_PIN
    );
}

static void real_start(void)
{
    servo_write(BOARD_SERVO_OPEN_ANGLE, "ABRIR/liberar");
    active = 1;
}

static void real_stop(void)
{
    servo_write(BOARD_SERVO_CLOSED_ANGLE, "FECHAR/parar");
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
    int channel = servo.attach(BOARD_SERVO_PIN);
    const char *kind = (channel >= 100) ? "MCPWM" : "LEDC";

    Serial.printf(
        "[SERVO] init pino=%d canal=%d (%s) aberto=%d fechado=%d\n",
        BOARD_SERVO_PIN,
        channel,
        kind,
        BOARD_SERVO_OPEN_ANGLE,
        BOARD_SERVO_CLOSED_ANGLE
    );

    servo_write(BOARD_SERVO_CLOSED_ANGLE, "FECHAR/inicial");
}