#ifndef IO_H
#define IO_H

#include <stdint.h>

// Функции для работы с портами ввода-вывода
void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
void outw(uint16_t port, uint16_t value);
uint16_t inw(uint16_t port);
void outl(uint16_t port, uint32_t value);
uint32_t inl(uint16_t port);

// Функции для работы с памятью ввода-вывода
void write_mem8(uint32_t address, uint8_t value);
uint8_t read_mem8(uint32_t address);
void write_mem16(uint32_t address, uint16_t value);
uint16_t read_mem16(uint32_t address);
void write_mem32(uint32_t address, uint32_t value);
uint32_t read_mem32(uint32_t address);

// Функции для работы с MSR (Model Specific Registers)
void write_msr(uint32_t msr, uint64_t value);
uint64_t read_msr(uint32_t msr);

// Функции для управления прерываниями
void enable_interrupts(void);
void disable_interrupts(void);

// Функции для работы с CR регистрами
void write_cr0(uint32_t value);
uint32_t read_cr0(void);
void write_cr3(uint32_t value);
uint32_t read_cr3(void);
void write_cr4(uint32_t value);
uint32_t read_cr4(void);

// Функция для остановки процессора
void halt(void);

#endif // IO_H
