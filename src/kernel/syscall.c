#include "syscall.h"
#include "interrupt.h"
#include "task.h"
#include "console.h"

#define SYSCALL_INT 0x80

static void syscall_handler(registers_t regs);

void syscall_init() {
    register_interrupt_handler(SYSCALL_INT, &syscall_handler);
    console_write("Syscall: Handler registered\n");
}

static void syscall_handler(registers_t regs) {
    // Номер системного вызова находится в EAX
    switch (regs.eax) {
        case SYS_EXIT:
            // Завершение текущего процесса
            console_write("Syscall: Exit called\n");
            // TODO: Реализовать завершение процесса
            break;
        case SYS_FORK:
            // Создание нового процесса
            regs.eax = fork();
            break;
        case SYS_READ:
            // Чтение из файла или устройства
            console_write("Syscall: Read called\n");
            // TODO: Реализовать чтение
            break;
        case SYS_WRITE:
            // Запись в файл или устройство
            console_write("Syscall: Write called\n");
            // Пример: запись в консоль
            console_write((const char*)regs.ebx);
            break;
        case SYS_OPEN:
            // Открытие файла
            console_write("Syscall: Open called\n");
            // TODO: Реализовать открытие файла
            break;
        case SYS_CLOSE:
            // Закрытие файла
            console_write("Syscall: Close called\n");
            // TODO: Реализовать закрытие файла
            break;
        default:
            console_write("Syscall: Unknown syscall\n");
            break;
    }
}

// Функции-обертки для системных вызовов

int sys_exit(int status) {
    int ret;
    asm volatile("int $0x80" : "=a" (ret) : "0" (SYS_EXIT), "b" (status));
    return ret;
}

int sys_fork() {
    int ret;
    asm volatile("int $0x80" : "=a" (ret) : "0" (SYS_FORK));
    return ret;
}

int sys_read(int fd, void *buf, size_t count) {
    int ret;
    asm volatile("int $0x80" : "=a" (ret) : "0" (SYS_READ), "b" (fd), "c" (buf), "d" (count));
    return ret;
}

int sys_write(int fd, const void *buf, size_t count) {
    int ret;
    asm volatile("int $0x80" : "=a" (ret) : "0" (SYS_WRITE), "b" (fd), "c" (buf), "d" (count));
    return ret;
}

int sys_open(const char *pathname, int flags) {
    int ret;
    asm volatile("int $0x80" : "=a" (ret) : "0" (SYS_OPEN), "b" (pathname), "c" (flags));
    return ret;
}

int sys_close(int fd) {
    int ret;
    asm volatile("int $0x80" : "=a" (ret) : "0" (SYS_CLOSE), "b" (fd));
    return ret;
}
