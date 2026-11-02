#ifndef TEST_ESP32_SERVO_H
#define TEST_ESP32_SERVO_H

extern int test_servo_angle;
extern int test_servo_pin;
extern int test_servo_writes;

class Servo {
public:
    int attach(int pin)
    {
        test_servo_pin = pin;
        return 100;
    }

    void write(int angle)
    {
        test_servo_angle = angle;
        test_servo_writes++;
    }

    int readMicroseconds()
    {
        return 544 + test_servo_angle * 1856 / 180;
    }
};

#endif
