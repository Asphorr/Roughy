#ifndef VGA_H
#define VGA_H

#include <stdint.h>
#include <stdbool.h>

// Стандартные режимы VGA
#define VGA_MODE_TEXT_80x25   0x03
#define VGA_MODE_GRAPHICS_320x200 0x13

// Цвета
typedef enum {
    VGA_COLOR_BLACK,
    VGA_COLOR_BLUE,
    VGA_COLOR_GREEN,
    VGA_COLOR_CYAN,
    VGA_COLOR_RED,
    VGA_COLOR_MAGENTA,
    VGA_COLOR_BROWN,
    VGA_COLOR_LIGHT_GREY,
    VGA_COLOR_DARK_GREY,
    VGA_COLOR_LIGHT_BLUE,
    VGA_COLOR_LIGHT_GREEN,
    VGA_COLOR_LIGHT_CYAN,
    VGA_COLOR_LIGHT_RED,
    VGA_COLOR_LIGHT_MAGENTA,
    VGA_COLOR_LIGHT_BROWN,
    VGA_COLOR_WHITE
} vga_color_t;

// Инициализация VGA
void vga_init(uint8_t mode);

// Функции для текстового режима
void vga_putchar(char c);
void vga_write(const char* str);
void vga_clear(void);
void vga_set_color(vga_color_t fg, vga_color_t bg);
void vga_set_cursor(uint8_t x, uint8_t y);

// Функции для графического режима
void vga_set_pixel(uint16_t x, uint16_t y, uint8_t color);
void vga_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);
void vga_draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color, bool fill);

// Переключение режимов
void vga_set_mode(uint8_t mode);

// Получение информации о текущем режиме
uint8_t vga_get_mode(void);
void vga_get_resolution(uint16_t* width, uint16_t* height);

#endif
