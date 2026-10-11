#if defined(ROLE_SCREEN) || defined(ROLE_EXTERNAL)

#include <Arduino.h>

#include "link_uart.h"

#if defined(ROLE_SCREEN)
#include "../hardware/esp32/board_config.h"
#else
#include "../hardware/esp32/board_config_ctrl.h"
#endif

static HardwareSerial link_serial(BOARD_LINK_UART_NUM);

extern "C" void link_uart_begin(void)
{
    link_serial.begin(
        BOARD_LINK_BAUD,
        SERIAL_8N1,
        BOARD_LINK_RX_PIN,
        BOARD_LINK_TX_PIN
    );
}

extern "C" int link_uart_write(const uint8_t *data, int len)
{
    return link_serial.write(data, (size_t)len);
}

extern "C" int link_uart_read(uint8_t *byte)
{
    if (link_serial.available() <= 0) {
        return 0;
    }

    *byte = (uint8_t)link_serial.read();

    return 1;
}

extern "C" LinkUart link_uart = {
    link_uart_begin,
    link_uart_write,
    link_uart_read
};

#endif
