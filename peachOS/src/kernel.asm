[BITS 32]
global _start

CODE_SEG equ 0x08 ; kernel code segment address
DATA_SEG equ 0x10 ; kernel data segment address

_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp

    ; enable the a20 line
    in al, 0x92
    or al, 2
    out 0x92, al

    jmp $
