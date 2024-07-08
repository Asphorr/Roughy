#include "console.h"
#include "../drivers/vga.h"
#include <stdarg.h>

static int cursor_x = 0;
static int cursor_y = 0;
static uint8_t console_color = 0x07; // Light grey on black

void console_init(void) {
    vga_init();
    console_clear();
}

void console_clear(void) {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            vga_putchar(' ', console_color, x, y);
        }
    }
    cursor_x = 0;
    cursor_y = 0;
}

void console_putchar(char c) {
    if (c == '\n') {
        cursor_y++;
        cursor_x = 0;
    } else if (c == '\r') {
        cursor_x = 0;
    } else {
        vga_putchar(c, console_color, cursor_x, cursor_y);
        cursor_x++;
    }

    if (cursor_x >= VGA_WIDTH) {
        cursor_y++;
        cursor_x = 0;
    }

    if (cursor_y >= VGA_HEIGHT) {
        vga_scroll();
        cursor_y = VGA_HEIGHT - 1;
    }
}

void console_write(const char* str) {
    while (*str) {
        console_putchar(*str++);
    }
}

void console_write_hex(uint32_t n) {
    int tmp;
    console_write("0x");
    char noZeroes = 1;

    for (int i = 28; i > 0; i -= 4) {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0) {
            continue;
        }
        if (tmp >= 0xA) {
            noZeroes = 0;
            console_putchar(tmp - 0xA + 'A');
        } else {
            noZeroes = 0;
            console_putchar(tmp + '0');
        }
    }
  
    tmp = n & 0xF;
    if (tmp >= 0xA) {
        console_putchar(tmp - 0xA + 'A');
    } else {
        console_putchar(tmp + '0');
    }
}

void console_set_color(uint8_t foreground, uint8_t background) {
    console_color = (background << 4) | (foreground & 0x0F);
}

void console_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    while (*format != '\0') {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 's':
                    console_write(va_arg(args, const char*));
                    break;
                case 'd':
                    // Implement integer to string conversion
                    break;
                case 'x':
                    console_write_hex(va_arg(args, uint32_t));
                    break;
                case 'c':
                    console_putchar((char)va_arg(args, int));
                    break;
                default:
                    console_putchar(*format);
            }
        } else {
            console_putchar(*format);
        }
        format++;
    }

    va_end(args);
}
