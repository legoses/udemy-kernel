section .asm

extern int21h_handler
extern no_interrupt_handler

global idt_load
global int21h
global no_interrupt

idt_load:
    push ebp
    mov ebp, esp

    mov ebx, [ebp+8] ; skip past return address and function address to first argument passwd to function
    lidt [ebx] ; loads the interrupt descriptor table

    pop ebp
    ret

int21h:
    cli ; clear interrupts, avoid nested interrupts, so processor doesnt get interrupted mid interrupt
    pushad ; pushes all general purpose registers

    call int21h_handler

    sti ; start interrupts
    popad ; restors all general purpose registers
    iret


no_interrupt: ; called for when interrupt does not exist/ not impemented yet, Avoid processor throwing fault and resetting
    cli ; clear interrupts, avoid nested interrupts, so processor doesnt get interrupted mid interrupt
    pushad ; pushes all general purpose registers

    call no_interrupt_handler

    sti ; start interrupts
    popad ; restors all general purpose registers
    iret
