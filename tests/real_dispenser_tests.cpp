#include <cstdio>
#include <cstdlib>

#include <Arduino.h>

#include "../src/hardware/dispenser.h"
#include "../src/hardware/esp32/board_config.h"
#include "../src/hardware/esp32/real_dispenser.h"

TestSerial Serial;
int test_servo_angle = -1;
int test_servo_pin = -1;
int test_servo_writes = 0;

static void check(bool condition, int line)
{
    if (!condition) {
        std::fprintf(stderr, "Falha na linha %d\n", line);
        std::exit(EXIT_FAILURE);
    }
}

#define CHECK(condition) check((condition), __LINE__)

int main()
{
    real_dispenser_init();
    CHECK(test_servo_pin == BOARD_SERVO_PIN);
    CHECK(test_servo_angle == BOARD_SERVO_CLOSED_ANGLE);
    CHECK(!dispenser.is_active());
    dispenser.start(DISPENSER_FLOW_FAST);
    CHECK(test_servo_angle == BOARD_SERVO_FAST_ANGLE);
    CHECK(dispenser.is_active());
    dispenser.start(DISPENSER_FLOW_FINE);
    CHECK(test_servo_angle == BOARD_SERVO_FINE_ANGLE);
    CHECK(dispenser.is_active());
    int writes = test_servo_writes;
    dispenser.start(DISPENSER_FLOW_FINE);
    CHECK(test_servo_writes == writes);
    dispenser.stop();
    CHECK(test_servo_angle == BOARD_SERVO_CLOSED_ANGLE);
    CHECK(!dispenser.is_active());
    dispenser.start(DISPENSER_FLOW_FINE);
    CHECK(test_servo_writes == writes + 2);
    CHECK(test_servo_angle == BOARD_SERVO_FINE_ANGLE);
    dispenser.stop();
    dispenser.start(DISPENSER_FLOW_MANUAL);
    CHECK(test_servo_angle == BOARD_SERVO_MANUAL_ANGLE);
    CHECK(dispenser.is_active());
    dispenser.start(static_cast<DispenserFlow>(-1));
    CHECK(test_servo_angle == BOARD_SERVO_CLOSED_ANGLE);
    CHECK(!dispenser.is_active());
    std::puts("Testes do driver de servo com substitutos: OK");
    return EXIT_SUCCESS;
}
