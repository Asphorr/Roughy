#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stdbool.h>

#define PAGE_SIZE 4096

typedef struct page {
    uint32_t present    : 1;
    uint32_t rw         : 1;
    uint32_t user       : 1;
    uint32_t accessed   : 1;
    uint32_t dirty      : 1;
    uint32_t unused     : 7;
    uint32_t frame      : 20;
} page_t;

typedef struct page_table {
    page_t pages[1024];
} page_table_t;

typedef struct page_directory {
    page_table_t *tables[1024];
    uint32_t tablesPhysical[1024];
    uint32_t physicalAddr;
} page_directory_t;

void memory_init(void);
void* kmalloc(uint32_t size);
void* kmalloc_a(uint32_t size);
void* kmalloc_p(uint32_t size, uint32_t *phys);
void* kmalloc_ap(uint32_t size, uint32_t *phys);
void kfree(void *p);
void page_fault(registers_t regs);

#endif // MEMORY_H
