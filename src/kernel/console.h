#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>

void console_init(void);
void console_clear(void);
void console_putchar(char c);
void console_write(const char* str);
void console_write_hex(uint32_t n);
void console_set_color(uint8_t foreground, uint8_t background);

#endif // CONSOLE_H
