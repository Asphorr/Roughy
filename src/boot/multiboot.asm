[BITS 32]

MBALIGN     equ 1<<0
MEMINFO     equ 1<<1
FLAGS       equ MBALIGN | MEMINFO
MAGIC       equ 0x1BADB002
CHECKSUM    equ -(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .bss
align 16
stack_bottom:
resb 16384 ; 16 KiB
stack_top:

section .text
global _start
_start:
    mov esp, stack_top

    ; Проверка магического числа Multiboot
    cmp eax, 0x2BADB002
    jne .no_multiboot

    ; Сохранение указателя на структуру Multiboot
    push ebx

    ; Вызов функции инициализации ядра
    extern kernel_main
    call kernel_main

    ; В случае возврата из kernel_main
    cli
.hang:
    hlt
    jmp .hang

.no_multiboot:
    ; Обработка ошибки, если загрузчик не совместим с Multiboot
    mov dword [0xB8000], 0x4F524F45
    mov dword [0xB8004], 0x4F3A4F52
    mov dword [0xB8008], 0x4F204F20
    mov dword [0xB800C], 0x4F4F4F4D
    mov dword [0xB8010], 0x4F544F42
    hlt
