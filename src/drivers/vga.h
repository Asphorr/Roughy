#ifndef VGA_H
#define VGA_H

#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void vga_init(void);
void vga_putchar(char c, uint8_t color, uint8_t x, uint8_t y);
void vga_clear(void);
void vga_scroll(void);

#endif // VGA_H
