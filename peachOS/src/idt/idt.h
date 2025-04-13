#ifndef IDT_H
#define IDT_H

#include <stdint.h>



/*
* | 63                 48 | 47 | 46 45 | 44 | 43    40 | 39           32 | 31                 16 | 15              0 |
*         offset             P    DPL     0   Gate type    Reserved            Segment selector       offset
* offset - 32 bit value split into two parts that represents the address of the entry point of an interrupt
* Selector - Has multiple fields and must point to a valid code segment in global descriptor table
* gate type - 4 bit value which represents which type of gate this descriptor represents. it has 5 valid vvalues
*   - 0b0101 or 0x5: take gate. With a task gate, the offset value is unused and should be set to zero
*   - 0b0110 or 0x6: 16 bit interrupt gate
*   - 0b0111 or 0x7: 16 bit trap gate
*   - 0b1110 or 0xE: 32 bit interrupt gate
*   - 0b1111 or 0xF: 32 bit trap gate
* DPL - 2 bit value which defines the cpu privilege levels. Kernel space vs user space
* P - present bit. must be set to 1 for the descriptor to be valid
*/

struct idt_desc {
    uint16_t offset_1; // offset bits 0 - 15
    uint16_t selector;  // selector thats in out gdt
    uint8_t zero; // Does nothing. Unused and set to zero
    uint8_t type_attr; // descriptor type and attributes
    uint16_t offset_2; // offset bits 16-31
} __attribute__((packed));


struct idtr_desc {
    uint16_t limit; // size of descriptor table - 1
    uint32_t base; // base address of the start of the table
} __attribute__((packed));


void idt_init();


#endif
