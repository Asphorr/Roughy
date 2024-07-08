#include "task.h"
#include "console.h"
#include "memory.h"

volatile task_t *current_task;
volatile task_t *ready_queue;

extern page_directory_t *kernel_directory;
extern page_directory_t *current_directory;
extern void alloc_frame(page_t*, int, int);
extern uint32_t initial_esp;
extern uint32_t read_eip();

uint32_t next_pid = 1;

void initialize_tasking() {
    asm volatile("cli");

    // Relocate the stack so we know where it is
    move_stack((void*)0xE0000000, 0x2000);

    // Initialize the first task (kernel task)
    current_task = ready_queue = (task_t*)kmalloc(sizeof(task_t));
    current_task->id = next_pid++;
    current_task->esp = current_task->ebp = 0;
    current_task->eip = 0;
    current_task->page_directory = current_directory;
    current_task->next = 0;
    current_task->kernel_stack = kmalloc_a(KERNEL_STACK_SIZE);

    asm volatile("sti");
}

int fork() {
    asm volatile("cli");

    // Take a pointer to this process' task struct for later reference
    task_t *parent_task = (task_t*)current_task;

    // Clone the address space
    page_directory_t *directory = clone_directory(current_directory);

    // Create a new process
    task_t *new_task = (task_t*)kmalloc(sizeof(task_t));
    new_task->id = next_pid++;
    new_task->esp = new_task->ebp = 0;
    new_task->eip = 0;
    new_task->page_directory = directory;
    new_task->kernel_stack = kmalloc_a(KERNEL_STACK_SIZE);
    new_task->next = 0;

    // Add it to the end of the ready queue
    task_t *tmp_task = (task_t*)ready_queue;
    while (tmp_task->next)
        tmp_task = tmp_task->next;
    tmp_task->next = new_task;

    // This will be the entry point for the new process
    uint32_t eip = read_eip();

    if (current_task == parent_task) {
        // We are the parent, so set up the esp/ebp/eip for our child
        uint32_t esp; asm volatile("mov %%esp, %0" : "=r"(esp));
        uint32_t ebp; asm volatile("mov %%ebp, %0" : "=r"(ebp));
        new_task->esp = esp;
        new_task->ebp = ebp;
        new_task->eip = eip;
        asm volatile("sti");

        return new_task->id;
    } else {
        // We are the child - by convention return 0
        return 0;
    }
}

void switch_task() {
    if (!current_task)
        return;

    uint32_t esp, ebp, eip;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));

    eip = read_eip();
    
    if (eip == 0x12345) // Magic number, means we've just switched tasks
        return;

    current_task->eip = eip;
    current_task->esp = esp;
    current_task->ebp = ebp;

    current_task = current_task->next;
    if (!current_task) current_task = ready_queue; //If we're at the end, start again

    esp = current_task->esp;
    ebp = current_task->ebp;

    current_directory = current_task->page_directory;
    
    asm volatile("         \
      mov %0, %%ecx;       \
      mov %1, %%esp;       \
      mov %2, %%ebp;       \
      mov %3, %%cr3;       \
      mov $0x12345, %%eax; \
      sti;                 \
      jmp *%%ecx           "
                : : "r"(eip), "r"(esp), "r"(ebp), "r"(current_directory->physicalAddr));
}

int getpid() {
    return current_task->id;
}
