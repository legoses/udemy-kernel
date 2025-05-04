[BITS 32]

section .asm

global paging_load_directory
global enable_paging

paging_load_directory:
    push ebp ; create stack pointer
    mov ebp, esp ; move base pointer to stack pointer register
    mov eax, [ebp+8] ; get argument passed (address to page directory)
    ; the cr3 register enalbes the processor to translate virtual address to physical ones by locating the page directory and page table for the current task
    mov cr3, eax ; mov address to directory to cr3 register
    pop ebp
    ret

enable_paging:
    push ebp
    mov ebp, esp

    ; move contents of cr0 to eax, since cr0 cannot be modified directly
    ; cr0 register contains configuration of processor operations such as enabling paging, cache, or protected mode
    mov eax, cr0
    or eax, 0x80000000 ; set bit to enable paging 
    mov cr0, eax
    pop ebp
    ret
