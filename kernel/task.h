#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include "memory.h"

#define KERNEL_STACK_SIZE 4096

typedef struct task {
    int id;
    uint32_t esp, ebp;
    uint32_t eip;
    page_directory_t *page_directory;
    uint32_t kernel_stack;
    struct task *next;
} task_t;

void task_init(void);
int fork(void);
void switch_task(void);
int getpid(void);

#endif // TASK_H
