#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>

// Скан-коды клавиш
#define KEY_ESCAPE  0x01
#define KEY_ENTER   0x1C
#define KEY_LSHIFT  0x2A
#define KEY_RSHIFT  0x36
#define KEY_LCTRL   0x1D
#define KEY_LALT    0x38

typedef void (*keyboard_callback_t)(uint8_t scancode);

void keyboard_init(void);
uint8_t keyboard_read(void);
char keyboard_scancode_to_ascii(uint8_t scancode);
bool keyboard_is_key_pressed(uint8_t scancode);
void keyboard_set_callback(keyboard_callback_t callback);
void keyboard_enable_interrupts(void);
void keyboard_disable_interrupts(void);

#endif
