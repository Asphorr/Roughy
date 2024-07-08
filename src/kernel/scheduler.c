#include "scheduler.h"
#include "task.h"
#include "console.h"
#include "memory.h"

static task_t* current_task = NULL;
static task_t* task_list = NULL;

void scheduler_init() {
    // Initialize the scheduler
    current_task = NULL;
    task_list = NULL;
    console_write("Scheduler: Initialized\n");
}

void scheduler_add_task(task_t* task) {
    if (!task_list) {
        task_list = task;
        task->next = NULL;
    } else {
        task->next = task_list;
        task_list = task;
    }
}

void scheduler_remove_task(task_t* task) {
    if (task == task_list) {
        task_list = task_list->next;
    } else {
        task_t* temp = task_list;
        while (temp->next != task) {
            temp = temp->next;
        }
        temp->next = task->next;
    }
}

void scheduler_run() {
    if (!current_task) {
        current_task = task_list;
    }

    if (current_task) {
        task_t* next_task = current_task->next;
        if (!next_task) {
            next_task = task_list;
        }
        task_switch(next_task);
    }
}

void task_switch(task_t* next) {
    if (next == current_task) {
        return;
    }

    task_t* prev = current_task;
    current_task = next;

    // Сохраняем контекст текущей задачи
    asm volatile(
        "pushf\n\t"                 // Сохраняем флаги
        "push %%cs\n\t"             // Сохраняем CS
        "push $1f\n\t"              // Сохраняем EIP (адрес метки 1)
        "push %%eax\n\t"
        "push %%ecx\n\t"
        "push %%edx\n\t"
        "push %%ebx\n\t"
        "push %%ebp\n\t"
        "push %%esi\n\t"
        "push %%edi\n\t"
        "mov %%esp, %0\n\t"         // Сохраняем ESP в структуре задачи
        "mov %1, %%esp\n\t"         // Загружаем ESP новой задачи
        "mov %2, %%cr3\n\t"         // Переключаем страничную директорию
        "pop %%edi\n\t"
        "pop %%esi\n\t"
        "pop %%ebp\n\t"
        "pop %%ebx\n\t"
        "pop %%edx\n\t"
        "pop %%ecx\n\t"
        "pop %%eax\n\t"
        "iret\n\t"                  // Восстанавливаем EIP, CS и флаги
        "1:\n\t"                    // Метка для возврата
        : "=m" (prev->esp)
        : "r" (next->esp), "r" (next->page_directory->physicalAddr)
        : "memory"
    );
}

// Вспомогательная функция для создания новой задачи
task_t* create_task(void (*entry_point)()) {
    task_t* new_task = (task_t*)kmalloc(sizeof(task_t));
    new_task->id = next_pid++;
    new_task->esp = (uint32_t)kmalloc(STACK_SIZE) + STACK_SIZE;  // Стек растет вниз
    new_task->ebp = new_task->esp;
    new_task->eip = (uint32_t)entry_point;
    new_task->page_directory = clone_directory(kernel_directory);
    new_task->next = NULL;

    // Настройка начального состояния стека
    uint32_t *stack = (uint32_t*)new_task->esp;
    *--stack = 0x202;          // EFLAGS
    *--stack = 0x08;           // CS
    *--stack = (uint32_t)entry_point;  // EIP
    *--stack = 0;              // EAX
    *--stack = 0;              // ECX
    *--stack = 0;              // EDX
    *--stack = 0;              // EBX
    *--stack = 0;              // ESP (будет заполнено позже)
    *--stack = 0;              // EBP
    *--stack = 0;              // ESI
    *--stack = 0;              // EDI
    new_task->esp = (uint32_t)stack;

    return new_task;
}

// Функция для запуска первой задачи
void start_multitasking() {
    asm volatile(
        "mov %0, %%esp\n\t"
        "pop %%edi\n\t"
        "pop %%esi\n\t"
        "pop %%ebp\n\t"
        "pop %%ebx\n\t"
        "pop %%edx\n\t"
        "pop %%ecx\n\t"
        "pop %%eax\n\t"
        "iret\n\t"
        : : "r" (current_task->esp)
    );
}
