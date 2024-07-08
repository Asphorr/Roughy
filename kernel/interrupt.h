#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

typedef struct {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

typedef void (*isr_t)(registers_t);

void interrupt_init(void);
void register_interrupt_handler(uint8_t n, isr_t handler);

#endif // INTERRUPT_H
