#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_COMMAND_PORT 0x64

void keyboard_init(void);
uint8_t keyboard_read_scan_code(void);
char keyboard_scan_code_to_ascii(uint8_t scan_code);

#endif // KEYBOARD_H
