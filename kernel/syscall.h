#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>

#define SYSCALL_MAX 128

void syscall_init(void);
int32_t syscall_handler(uint32_t syscall_number, uint32_t arg1, uint32_t arg2, uint32_t arg3);

// Определения системных вызовов
#define SYS_EXIT    1
#define SYS_FORK    2
#define SYS_READ    3
#define SYS_WRITE   4
#define SYS_OPEN    5
#define SYS_CLOSE   6

#endif // SYSCALL_H
