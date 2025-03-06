ORG 0x7c00 ; 
BITS 16 ; specify this is 16 bit code

start:
    mov si, message
    call print
    jmp $ ; jump to self. $ likely means current memory addressm and $$ means starting?

print:
    mov bx, 0
.loop:
    lodsb ; loads a single byte from the si register into al register
    cmp al, 0
    je .done ; if al equals zero, jump to done

    ; else
    call print_char
    jmp .loop
.done:
    ret

print_char:
    mov ah, 0eh ; move 0eh into ah register. Command to priunt to the screen
    int 0x10 ; interrupt, calls the BIOS. Ralph brown interrupt list. int seems to work like syscall
    ret


message: db 'Hello, World!', 0
times 510-($ - $$) db 0 ; specified we need ot fill at least 510 bytes of data. if our code does not fill 510 bytes, this will pad the rest with 0's
; dw will append this value to the end of this file. dw stands for 2 bytes
dw 0xAA55 ; boot signature. Backwards because intel is little endian, so bytes get flipped when used
