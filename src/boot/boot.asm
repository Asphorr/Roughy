[BITS 16]
[ORG 0x7C00]

start:
    ; Инициализация сегментных регистров
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Вывод приветственного сообщения
    mov si, welcome_msg
    call print_string

    ; Загрузка второй стадии
    mov ah, 0x02
    mov al, 1        ; Количество секторов для чтения
    mov ch, 0        ; Цилиндр 0
    mov cl, 2        ; Сектор 2 (сразу после загрузочного сектора)
    mov dh, 0        ; Головка 0
    mov bx, 0x7E00   ; Адрес для загрузки
    int 0x13

    ; Переход ко второй стадии
    jmp 0x7E00

print_string:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp print_string
.done:
    ret

welcome_msg db 'Booting...', 0x0D, 0x0A, 0

times 510-($-$$) db 0
dw 0xAA55
