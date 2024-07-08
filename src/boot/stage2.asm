[BITS 16]

start:
    ; Вывод сообщения о второй стадии
    mov si, stage2_msg
    call print_string

    ; Проверка поддержки A20 линии
    call check_a20
    test ax, ax
    jz .no_a20

    ; Загрузка GDT
    cli
    lgdt [gdt_descriptor]

    ; Переключение в защищенный режим
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Дальний прыжок для обновления CS
    jmp 0x08:protected_mode

[BITS 32]
protected_mode:
    ; Инициализация сегментных регистров
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Настройка стека
    mov esp, 0x90000

    ; Переход к коду ядра
    jmp 0x100000

[BITS 16]
print_string:
    ; (тот же код, что и в boot.asm)

check_a20:
    pushf
    push ds
    push es
    push di
    push si

    cli

    xor ax, ax
    mov es, ax
    not ax
    mov ds, ax

    mov di, 0x0500
    mov si, 0x0510

    mov al, byte [es:di]
    push ax

    mov al, byte [ds:si]
    push ax

    mov byte [es:di], 0x00
    mov byte [ds:si], 0xFF

    cmp byte [es:di], 0xFF

    pop ax
    mov byte [ds:si], al
    pop ax
    mov byte [es:di], al

    mov ax, 0
    je .done

    mov ax, 1

.done:
    pop si
    pop di
    pop es
    pop ds
    popf
    ret

.no_a20:
    mov si, no_a20_msg
    call print_string
    jmp $

stage2_msg db 'Stage 2 loaded', 0x0D, 0x0A, 0
no_a20_msg db 'A20 line not enabled', 0x0D, 0x0A, 0

gdt_start:
    dq 0
gdt_code:
    dw 0xFFFF
    dw 0
    db 0
    db 10011010b
    db 11001111b
    db 0
gdt_data:
    dw 0xFFFF
    dw 0
    db 0
    db 10010010b
    db 11001111b
    db 0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 512-($-$$) db 0
