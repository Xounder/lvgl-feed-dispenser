#include "real_dispenser.h"

#include <Arduino.h>
#include <ESP32Servo.h>

#include "../dispenser.h"
#include "board_config.h"

static_assert(BOARD_SERVO_CLOSED_ANGLE >= 0 && BOARD_SERVO_CLOSED_ANGLE <= 180,
    "Angulo fechado fora da faixa do servo");
static_assert(BOARD_SERVO_FAST_ANGLE >= 0 && BOARD_SERVO_FAST_ANGLE <= 180,
    "Angulo rapido fora da faixa do servo");
static_assert(BOARD_SERVO_MANUAL_ANGLE >= 0 && BOARD_SERVO_MANUAL_ANGLE <= 180,
    "Angulo manual fora da faixa do servo");
static_assert((BOARD_SERVO_FINE_ANGLE - BOARD_SERVO_CLOSED_ANGLE) *
    (BOARD_SERVO_FINE_ANGLE - BOARD_SERVO_FAST_ANGLE) < 0,
    "Angulo fino deve ficar entre fechado e rapido");

static Servo servo;
static int active = 0;
static DispenserFlow current_flow = DISPENSER_FLOW_FAST;

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

static void real_stop(void)
{
    servo_write(BOARD_SERVO_CLOSED_ANGLE, "FECHAR/parar");
    active = 0;
}

static void real_start(DispenserFlow flow)
{
    int angle;
    const char *action;

    switch (flow) {
        case DISPENSER_FLOW_FAST:
            angle = BOARD_SERVO_FAST_ANGLE;
            action = "ABRIR/rapida";
            break;

        case DISPENSER_FLOW_FINE:
            angle = BOARD_SERVO_FINE_ANGLE;
            action = "REDUZIR/fina";
            break;

        case DISPENSER_FLOW_MANUAL:
            angle = BOARD_SERVO_MANUAL_ANGLE;
            action = "ABRIR/manual";
            break;

        default:
            real_stop();
            return;
    }

    if (active && current_flow == flow) {
        return;
    }

    servo_write(angle, action);
    current_flow = flow;
    active = 1;
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
        "[SERVO] init pino=%d canal=%d (%s) rapida=%d fina=%d manual=%d fechado=%d\n",
        BOARD_SERVO_PIN,
        channel,
        kind,
        BOARD_SERVO_FAST_ANGLE,
        BOARD_SERVO_FINE_ANGLE,
        BOARD_SERVO_MANUAL_ANGLE,
        BOARD_SERVO_CLOSED_ANGLE
    );

    servo_write(BOARD_SERVO_CLOSED_ANGLE, "FECHAR/inicial");
    active = 0;
}
