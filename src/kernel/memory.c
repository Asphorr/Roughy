#include "memory.h"
#include "console.h"

extern uint32_t end; // Определено в линкер-скрипте
uint32_t placement_address = (uint32_t)&end;
page_directory_t *kernel_directory = 0;
page_directory_t *current_directory = 0;

static void page_fault(registers_t regs);

void memory_init(void) {
    // Инициализация менеджера физической памяти
    uint32_t mem_end_page = 0x1000000; // Предполагаем 16 МБ RAM

    nframes = mem_end_page / 0x1000;
    frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(nframes));
    memset(frames, 0, INDEX_FROM_BIT(nframes));

    // Создаем директорию страниц ядра
    kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
    memset(kernel_directory, 0, sizeof(page_directory_t));
    current_directory = kernel_directory;

    // Идентично отображаем память ядра
    for (uint32_t i = 0; i < placement_address; i += 0x1000)
        alloc_frame(get_page(i, 1, kernel_directory), 0, 0);

    // Регистрируем обработчик page fault
    register_interrupt_handler(14, page_fault);

    // Включаем страничную адресацию
    switch_page_directory(kernel_directory);

    console_write("Memory: Paging enabled\n");
}

void switch_page_directory(page_directory_t *dir) {
    current_directory = dir;
    asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
    uint32_t cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t *get_page(uint32_t address, int make, page_directory_t *dir) {
    address /= 0x1000;
    uint32_t table_idx = address / 1024;
    if (dir->tables[table_idx]) {
        return &dir->tables[table_idx]->pages[address % 1024];
    } else if(make) {
        uint32_t tmp;
        dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
        memset(dir->tables[table_idx], 0, 0x1000);
        dir->tablesPhysical[table_idx] = tmp | 0x7; // Present, RW, US.
        return &dir->tables[table_idx]->pages[address % 1024];
    } else {
        return 0;
    }
}

void alloc_frame(page_t *page, int is_kernel, int is_writeable) {
    if (page->frame != 0) {
        return;
    } else {
        uint32_t idx = first_frame();
        if (idx == (uint32_t)-1) {
            // PANIC! no free frames!!
            console_write("Memory: PANIC! No free frames!\n");
            for(;;);
        }
        set_frame(idx * 0x1000);
        page->present = 1;
        page->rw = (is_writeable) ? 1 : 0;
        page->user = (is_kernel) ? 0 : 1;
        page->frame = idx;
    }
}

void free_frame(page_t *page) {
    uint32_t frame;
    if (!(frame = page->frame)) {
        return;
    } else {
        clear_frame(frame);
        page->frame = 0x0;
    }
}

static void page_fault(registers_t regs) {
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    int present   = !(regs.err_code & 0x1);
    int rw = regs.err_code & 0x2;
    int us = regs.err_code & 0x4;
    int reserved = regs.err_code & 0x8;
    int id = regs.err_code & 0x10;

    console_write("Memory: Page fault! ( ");
    if (present) console_write("present ");
    if (rw) console_write("read-only ");
    if (us) console_write("user-mode ");
    if (reserved) console_write("reserved ");
    console_write(") at 0x");
    console_write_hex(faulting_address);
    console_write("\n");

    // Здесь можно добавить более сложную обработку page fault
    for(;;);
}

void *kmalloc(uint32_t sz) {
    void *addr = (void*)placement_address;
    placement_address += sz;
    return addr;
}

void *kmalloc_a(uint32_t sz) {
    if (placement_address & 0xFFFFF000) {
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    void *addr = (void*)placement_address;
    placement_address += sz;
    return addr;
}

void *kmalloc_p(uint32_t sz, uint32_t *phys) {
    void *addr = kmalloc(sz);
    *phys = (uint32_t)addr;
    return addr;
}

void *kmalloc_ap(uint32_t sz, uint32_t *phys) {
    void *addr = kmalloc_a(sz);
    *phys = (uint32_t)addr;
    return addr;
}
