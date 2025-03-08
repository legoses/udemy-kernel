; look into segment registers
ORG 0 ; 
BITS 16 ; specify this is 16 bit code
_start:
    ; short jmp are usually used for jumping somehwere in the same module
    ; 2 byte instruction, literally jumping a short distance
    ; often uses a relative offet, so it is easier to relocate in memory
    jmp short start
    nop ; no operation

; create empty bytes for bios boot parameter block
; can safely be overwritten
times 33 db 0 ; created 33 bytes after short jump
start:
    jmp 0x7c0:step2; starts program at a specific memory address

step2:
    ; interrupts cause the processor to stop what it is going and execute the code handed to it
    cli ; clear interrupts so we are not interrupted while setting up emmory segments
    ; after we cleard interrupts, we can manually set the origin instead of hopign the bios has properly set it
    mov ax, 0x7c0
    mov ds, ax ; move 0x7c0 into data segmeht
    mov es, ax ; move 0x7c0 into extra segment
    mov ax, 0x00
    mov ss, ax ; set stack segment to 0
    mov sp, 0x7c00 ; set stack pointer to 0x7c00

    sti ; enables interrupts

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
