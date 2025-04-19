section .asm

global insb
global insw
global outb
global outw

insb: ; bring in single byte
    push ebp ; push base pointer of frame to the stack
    
    mov ebp, esp ; move current stack poitner to base pointer register

    ; with using c and assemblky together, eax as alwayas the register that contains the return value
    xor eax, eax ; xor with its self so it becomes zero
    mov edx, [ebp+8] ; put port in edx register. This moves the memory contents stored at ebt + 8 (probably parameter passed to this functuion) to the edx register
    in al, dx ; al is part of the eax register

    pop ebp ; restore old base pointer
    ret

insw: ; bring in a word (two bytes)
    push ebp
    mov ebp, esp

    xor eax, eax ; zero eax register
    mov edx, [ebp+8] ; get parameter passed
    in ax, dx

    pop ebp
    ret


outb: ; output a single byte
    push ebp
    mov ebp, esp

    ; get both parameters passed to function
    mov eax, [ebp+12]
    mov edx, [ebp+8]
    out dx, al

    pop ebp 
    ret


outw: ; output a word (two bytes)
    push ebp
    mov ebp, esp

    mov eax, [ebp+12]
    mov edx, [ebp+8]
    out dx, ax

    pop ebp
    ret
