#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

void keyboard_init(void);
char keyboard_getchar(void);
void keyboard_handler(registers_t regs);

#endif // KEYBOARD_H
