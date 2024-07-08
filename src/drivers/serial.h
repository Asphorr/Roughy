#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <stdbool.h>

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

typedef enum {
    BAUD_115200 = 1,
    BAUD_57600  = 2,
    BAUD_38400  = 3,
    BAUD_19200  = 6,
    BAUD_9600   = 12
} serial_baud_t;

void serial_init(uint16_t port, serial_baud_t baud);
bool serial_received(uint16_t port);
char serial_read(uint16_t port);
bool serial_is_transmit_empty(uint16_t port);
void serial_write(uint16_t port, char c);
void serial_write_string(uint16_t port, const char* str);
void serial_configure(uint16_t port, uint8_t data_bits, char parity, uint8_t stop_bits);
void serial_enable_interrupts(uint16_t port);
void serial_disable_interrupts(uint16_t port);

#endif
