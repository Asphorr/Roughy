#include <stdbool.h>
#include "console.h"
#include "memory.h"
#include "interrupt.h"
#include "task.h"
#include "scheduler.h"
#include "syscall.h"
#include "keyboard.h"
#include "timer.h"
#include "../drivers/vga.h"
#include "../drivers/pci.h"
#include "../fs/vfs.h"

extern void gdt_install(void);
extern void idt_install(void);

void kernel_main(void) {
    // Инициализация базовых компонентов
    gdt_install();
    idt_install();
    console_init();

    console_write("Kernel: GDT and IDT installed\n");

    // Инициализация памяти
    memory_init();
    console_write("Kernel: Memory management initialized\n");

    // Инициализация прерываний
    interrupt_init();
    console_write("Kernel: Interrupts initialized\n");

    // Инициализация устройств
    keyboard_init();
    console_write("Kernel: Keyboard initialized\n");

    timer_init(100); // 100 Hz
    console_write("Kernel: Timer initialized\n");

    // Инициализация многозадачности
    task_init();
    scheduler_init();
    console_write("Kernel: Multitasking system initialized\n");

    // Инициализация системных вызовов
    syscall_init();
    console_write("Kernel: System calls initialized\n");

    // Инициализация PCI
    pci_init();
    console_write("Kernel: PCI bus scanned\n");

    // Инициализация файловой системы
    vfs_init();
    console_write("Kernel: Virtual File System initialized\n");

    // Разрешаем прерывания
    asm volatile("sti");

    console_write("Kernel: Initialization complete. Starting scheduler...\n");

    // Запускаем планировщик
    while (true) {
        scheduler_run();
    }
}

void kernel_panic(const char* message) {
    console_write("KERNEL PANIC: ");
    console_write(message);
    console_write("\n");
    
    // Отключаем прерывания и останавливаем систему
    asm volatile("cli");
    for(;;);
}
