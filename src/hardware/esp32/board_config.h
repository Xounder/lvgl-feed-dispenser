#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

/* Pinos provisorios; validar contra a montagem fisica e o exemplo do fabricante. */

#define BOARD_DISPLAY_HOR_RES 800
#define BOARD_DISPLAY_VER_RES 480

#define BOARD_TOUCH_I2C_SDA_PIN 38
#define BOARD_TOUCH_I2C_SCL_PIN 39
#define BOARD_TOUCH_I2C_ADDR 0x5D

#define BOARD_HX711_DATA_PIN 6
#define BOARD_HX711_SCK_PIN 5
#define BOARD_HX711_CALIBRATION_FACTOR 1.0f

#define BOARD_SERVO_PIN 17
#define BOARD_SERVO_CLOSED_ANGLE 0
#define BOARD_SERVO_OPEN_ANGLE 90

#define BOARD_BUTTON_1_PIN 18
#define BOARD_BUTTON_2_PIN 21

#define BOARD_LED_PIN 2

#endif