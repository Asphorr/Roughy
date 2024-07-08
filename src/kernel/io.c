#include "io.h"

void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void outw(uint16_t port, uint16_t value) {
    asm volatile ("outw %0, %1" : : "a"(value), "Nd"(port));
}

uint16_t inw(uint16_t port) {
    uint16_t ret;
    asm volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void outl(uint16_t port, uint32_t value) {
    asm volatile ("outl %0, %1" : : "a"(value), "Nd"(port));
}

uint32_t inl(uint16_t port) {
    uint32_t ret;
    asm volatile ("inl %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void write_mem8(uint32_t address, uint8_t value) {
    *((volatile uint8_t*)address) = value;
}

uint8_t read_mem8(uint32_t address) {
    return *((volatile uint8_t*)address);
}

void write_mem16(uint32_t address, uint16_t value) {
    *((volatile uint16_t*)address) = value;
}

uint16_t read_mem16(uint32_t address) {
    return *((volatile uint16_t*)address);
}

void write_mem32(uint32_t address, uint32_t value) {
    *((volatile uint32_t*)address) = value;
}

uint32_t read_mem32(uint32_t address) {
    return *((volatile uint32_t*)address);
}

void write_msr(uint32_t msr, uint64_t value) {
    uint32_t low = (uint32_t)value;
    uint32_t high = (uint32_t)(value >> 32);
    asm volatile ("wrmsr" : : "c"(msr), "a"(low), "d"(high));
}

uint64_t read_msr(uint32_t msr) {
    uint32_t low, high;
    asm volatile ("rdmsr" : "=a"(low), "=d"(high) : "c"(msr));
    return ((uint64_t)high << 32) | low;
}

void enable_interrupts(void) {
    asm volatile ("sti");
}

void disable_interrupts(void) {
    asm volatile ("cli");
}

void write_cr0(uint32_t value) {
    asm volatile ("mov %0, %%cr0" : : "r"(value));
}

uint32_t read_cr0(void) {
    uint32_t value;
    asm volatile ("mov %%cr0, %0" : "=r"(value));
    return value;
}

void write_cr3(uint32_t value) {
    asm volatile ("mov %0, %%cr3" : : "r"(value));
}

uint32_t read_cr3(void) {
    uint32_t value;
    asm volatile ("mov %%cr3, %0" : "=r"(value));
    return value;
}

void write_cr4(uint32_t value) {
    asm volatile ("mov %0, %%cr4" : : "r"(value));
}

uint32_t read_cr4(void) {
    uint32_t value;
    asm volatile ("mov %%cr4, %0" : "=r"(value));
    return value;
}

void halt(void) {
    asm volatile ("hlt");
}
