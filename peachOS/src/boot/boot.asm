; look into segment registers
ORG 0x7c00 ; 
BITS 16 ; specify this is 16 bit code

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

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
    jmp 0:step2; starts program at a specific memory address

;   AH = 02h
;   AL = number of sectors to read (must be nonzero)
;   CH = low eight bits of cylinder number
;   CL = sector number 1-63 (bits 0-5)
;   high two bits of cylinder (bits 6-7, hard disk only)
;   DH = head number
;   DL = drive number (bit 7 set for hard disk)
;   ES:BX -> data buffer
;
;   Return:
;   CF set on error
;   if AH = 11h (corrected ECC error), AL = burst length
;   CF clear if successful
;   AH = status (see #00234)
;   AL = number of sectors transferred (only valid if CF set for some
;   BIOSes)

step2:
    ; interrupts cause the processor to stop what it is going and execute the code handed to it
    cli ; clear interrupts so we are not interrupted while setting up emmory segments
    ; after we cleard interrupts, we can manually set the origin instead of hopign the bios has properly set it
    mov ax, 0x00
    mov ds, ax ; move 0x7c0 into data segmeht
    mov es, ax ; move 0x7c0 into extra segment
    mov ss, ax ; set stack segment to 0
    mov sp, 0x7c00 ; set stack pointer to 0x7c00

    sti ; enables interrupts


.load_protected:
    cli ; clear interrupts
    lgdt[gdt_descriptor] ; load global descriptor table
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    ; jmp CODE_SEG:load32 ; jmp to address stored at code_seg selector defined at the top of the file
    jmp $

; GDT (GLobal Descriptor Table)
; contains enteries telling the CPU about memoryu segments

gdt_start: ; address to descript the start of the GDT table
gdt_null: ; 64 bits of 0
    dd 0x0
    dd 0x0

; offset 0x8
gdt_code: ; CS should point to this ; enteries are 8 bytes. mostly default values
    dw 0xffff ; segment limit first 0-15 bits
    dw 0  ; base 0-15 bits
    db 0   ; base 16-23 bits

    db 0x9a  ; access byte a bitmask, can also set configuration
    db 1100111b ; high 4 bit flags and low 4 bit flags
    db 0  ; base 24-31 bits

; offset 0x10
gdt_data: ; should be linked to DS, SS, EF, FS, GS
    dw 0xffff ; segment limit first 0-15 bits
    dw 0  ; base 0-15 bits
    db 0   ; base 16-23 bits

    db 0x92  ; access byte a bitmask, can also set configuration
    db 1100111b ; high 4 bit flags and low 4 bit flags
    db 0  ; base 24-31 bits

gdt_end: ; lable to describe the end address of the table

gdt_descriptor:
    dw gdt_end - gdt_start-1 ; get size of descriptorp
    dd gdt_start




times 510-($ - $$) db 0 ; specified we need ot fill at least 510 bytes of data. if our code does not fill 510 bytes, this will pad the rest with 0's
; dw will append this value to the end of this file. dw stands for 2 bytes
dw 0xAA55 ; boot signature. Backwards because intel is little endian, so bytes get flipped when used

; bios only loads one sector, so this will never be loaded
; by it can be referenced with a label
buffer: ; what we will write to. Placed at the end to make sure out code is not overwritten
