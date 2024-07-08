#include "keyboard.h"
#include "interrupt.h"
#include "console.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_COMMAND_PORT 0x64

static void keyboard_callback(registers_t regs);

// Простая таблица соответствия скан-кодов ASCII-символам
static char scancode_to_ascii[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

void keyboard_init() {
    register_interrupt_handler(IRQ1, &keyboard_callback);
    console_write("Keyboard: Initialized\n");
}

static void keyboard_callback(registers_t regs) {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    
    if (scancode & 0x80) {
        // Key release
    } else {
        // Key press
        if (scancode < sizeof(scancode_to_ascii)) {
            char c = scancode_to_ascii[scancode];
            if (c) {
                char str[2] = {c, '\0'};
                console_write(str);
            }
        }
    }
}

char keyboard_getchar() {
    char c = 0;
    while (c == 0) {
        uint8_t scancode = inb(KEYBOARD_DATA_PORT);
        if (!(scancode & 0x80) && scancode < sizeof(scancode_to_ascii)) {
            c = scancode_to_ascii[scancode];
        }
    }
    return c;
}
