#include "vga.h"
#include "../lib/io.h"
#include <string.h>

static uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;
static uint8_t vga_x = 0, vga_y = 0;
static uint8_t vga_color = 0x07; // Light grey on black
static uint8_t current_mode = VGA_MODE_TEXT_80x25;

void vga_init(uint8_t mode) {
    vga_set_mode(mode);
}

void vga_set_mode(uint8_t mode) {
    outb(0x3C2, 0x63);
    outb(0x3D4, 0x00);
    outb(0x3D5, 0x63);
    // ... (дополнительные настройки регистров для установки режима)
    current_mode = mode;
    vga_clear();
}

void vga_putchar(char c) {
    if (current_mode != VGA_MODE_TEXT_80x25) return;

    if (c == '\n') {
        vga_x = 0;
        if (++vga_y == 25) {
            // Scroll
            memmove(VGA_MEMORY, VGA_MEMORY + 80, 80 * 24 * 2);
            memset(VGA_MEMORY + 80 * 24, 0, 80 * 2);
            vga_y = 24;
        }
    } else {
        VGA_MEMORY[vga_y * 80 + vga_x] = (vga_color << 8) | c;
        if (++vga_x == 80) {
            vga_x = 0;
            if (++vga_y == 25) {
                // Scroll
                memmove(VGA_MEMORY, VGA_MEMORY + 80, 80 * 24 * 2);
                memset(VGA_MEMORY + 80 * 24, 0, 80 * 2);
                vga_y = 24;
            }
        }
    }
}

void vga_write(const char* str) {
    while (*str) {
        vga_putchar(*str++);
    }
}

void vga_clear(void) {
    if (current_mode == VGA_MODE_TEXT_80x25) {
        memset(VGA_MEMORY, 0, 80 * 25 * 2);
    } else {
        // Clear graphics mode
        memset((void*)0xA0000, 0, 320 * 200);
    }
    vga_x = vga_y = 0;
}

void vga_set_color(vga_color_t fg, vga_color_t bg) {
    vga_color = (bg << 4) | fg;
}

void vga_set_cursor(uint8_t x, uint8_t y) {
    uint16_t pos = y * 80 + x;
    outb(0x3D4, 14);
    outb(0x3D5, pos >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}

void vga_set_pixel(uint16_t x, uint16_t y, uint8_t color) {
    if (current_mode != VGA_MODE_GRAPHICS_320x200) return;
    uint8_t* vram = (uint8_t*)0xA0000;
    vram[y * 320 + x] = color;
}

void vga_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color) {
    // Реализация алгоритма Брезенхэма
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        vga_set_pixel(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

void vga_draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color, bool fill) {
    if (fill) {
        for (uint16_t i = y; i < y + height; i++) {
            for (uint16_t j = x; j < x + width; j++) {
                vga_set_pixel(j, i, color);
            }
        }
    } else {
        vga_draw_line(x, y, x + width - 1, y, color);
        vga_draw_line(x, y + height - 1, x + width - 1, y + height - 1, color);
        vga_draw_line(x, y, x, y + height - 1, color);
        vga_draw_line(x + width - 1, y, x + width - 1, y + height - 1, color);
    }
}

uint8_t vga_get_mode(void) {
    return current_mode;
}

void vga_get_resolution(uint16_t* width, uint16_t* height) {
    if (current_mode == VGA_MODE_TEXT_80x25) {
        *width = 80;
        *height = 25;
    } else if (current_mode == VGA_MODE_GRAPHICS_320x200) {
        *width = 320;
        *height = 200;
    }
}
