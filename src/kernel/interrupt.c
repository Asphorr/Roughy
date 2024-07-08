#include "interrupt.h"
#include "console.h"

isr_t interrupt_handlers[256];

void interrupt_init(void) {
    // Инициализация IDT происходит в asm файле
    console_write("Interrupts: Initialized\n");
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void isr_handler(registers_t regs) {
    console_write("Received interrupt: ");
    console_write_hex(regs.int_no);
    console_write("\n");

    if (interrupt_handlers[regs.int_no] != 0) {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}

void irq_handler(registers_t regs) {
    // Отправляем сигнал EOI (End of Interrupt) контроллеру прерываний
    if (regs.int_no >= 40) {
        // Отправляем контроллеру PIC2
        outb(0xA0, 0x20);
    }
    // Отправляем контроллеру PIC1
    outb(0x20, 0x20);

    if (interrupt_handlers[regs.int_no] != 0) {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}
