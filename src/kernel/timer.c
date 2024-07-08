#include "timer.h"
#include "interrupt.h"
#include "console.h"

#define PIT_CHANNEL0_DATA 0x40
#define PIT_COMMAND 0x43

static uint32_t tick = 0;

static void timer_callback(registers_t regs) {
    tick++;
    if (tick % 100 == 0) {  // Каждую секунду (при частоте 100 Гц)
        console_write("Timer: One second passed\n");
    }
}

void timer_init(uint32_t frequency) {
    register_interrupt_handler(IRQ0, &timer_callback);

    uint32_t divisor = 1193180 / frequency;
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    outb(PIT_COMMAND, 0x36);  // Command byte
    outb(PIT_CHANNEL0_DATA, low);
    outb(PIT_CHANNEL0_DATA, high);

    console_write("Timer: Initialized\n");
}

uint32_t timer_get_ticks() {
    return tick;
}
