[BITS 32]

global _start

extern kernel_start

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

    ; Remap master PIC https://pdos.csail.mit.edu/6.828/2010/readings/hardware/8259A.pdf
    mov al, 00010001b ; level triggered mode, ICW4 (initinization command word) needed
    out 0x20, al ; tell master pic

    mov al, 0x20 ; Interrupt 0x20 is where master ISR should start
    out 0x21, al

    mov al, 00000001b ; ICW2, write new interrupt vector address
    out 0x21, al
    ; end remap master PIC


    call kernel_start
    jmp $


times 512-($ - $$) db 0 ; padd out the section to make sure aligment with c file is correct
