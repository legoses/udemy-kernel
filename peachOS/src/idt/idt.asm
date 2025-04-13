section .asm

global idt_load
idt_load:
    push ebp
    mov ebp, esp

    mov ebx, [ebp+8] ; skip past return address and function address to first argument passwd to function
    lidt [ebx] ; loads the interrupt descriptor table

    pop ebp
    ret
