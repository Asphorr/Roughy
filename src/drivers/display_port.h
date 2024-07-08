#ifndef DISPLAY_PORT_H
#define DISPLAY_PORT_H

#include <stdint.h>
#include <stdbool.h>

// Поддерживаемые разрешения
#define DP_RES_1920x1080 0
#define DP_RES_2560x1440 1
#define DP_RES_3840x2160 2

typedef struct {
    uint16_t width;
    uint16_t height;
    uint8_t  bits_per_pixel;
    uint32_t refresh_rate;
} dp_mode_t;

void dp_init(void);
bool dp_set_mode(uint8_t resolution_mode);
void dp_get_current_mode(dp_mode_t* mode);
void dp_set_pixel(uint32_t x, uint32_t y, uint32_t color);
void dp_draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void dp_clear_screen(uint32_t color);
bool dp_is_connected(void);
void dp_enable_vsync(bool enable);

#endif
