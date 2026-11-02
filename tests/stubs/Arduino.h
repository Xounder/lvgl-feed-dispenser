#ifndef TEST_ARDUINO_H
#define TEST_ARDUINO_H

struct TestSerial {
    int printf(const char *, ...) { return 0; }
};

extern TestSerial Serial;

#endif
