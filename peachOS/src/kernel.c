#include "kernel.h"
#include <stdint.h>
#include <stddef.h>
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "string/string.h"
#include "fs/file.h"
#include "disk/disk.h"
#include "fs/pparser.h"
#include "disk/streamer.h"


uint16_t *video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;


// combine character and color into single 16 bit value
uint16_t terminal_make_char(char c, char color) {
    return (color << 8) | c;
}




// print a character to the screen with specified location and color
void terminal_putchar(int x, int y, char c, char color) {
    video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c, color);
}


// write characters with uniform color and position
void terminal_writechar(char c, char color) {
    if(c == '\n') { // implement newline
        terminal_row += 1;
        terminal_col = 0;
        return;
    }

    terminal_putchar(terminal_col, terminal_row, c, color);
    terminal_col += 1;
    if(terminal_col >= VGA_WIDTH) {
        terminal_col = 0;
        terminal_row += 1;
    }
}


// initialize terminal to get rid of sea bios output
void terminal_initialize() {
    video_mem = (uint16_t*)(0xB8000); // create character array that is stored at this address in order to print characters to the screen
    terminal_row = 0;
    terminal_col = 0;

    //convert x and y coords to store in 1 dimensional array
    for(int y = 0; y < VGA_HEIGHT; y++) {
        for(int x = 0; x < VGA_WIDTH; x++) {
            //video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(' ', 0); // store spaces in array
            terminal_putchar(x, y, ' ', 0);
        }
    }

}



// write a string to the screen
void print(const char *str) {
    size_t len = strlen(str);

    for(int i = 0; i < len; i++) {
        terminal_writechar(str[i], 15);
    }
}


static struct paging_4gb_chunk *kernel_chunk = 0;

void kernel_start() {
    // video_mem[0] = 0x0341; color and ascii code appear reversed because of endianess
    terminal_initialize();
    print("Hello, world!\ntest");

    // initialize heap
    kheap_init();
    
    // init file system
    fs_init();

    // search and initialize disks
    disk_search_and_init();

    idt_init(); // initialize interrupt descriptor table
    
    // setup paging
    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL); // enable paging
    paging_switch(paging_4gb_chunk_get_directory(kernel_chunk)); // get directory from 4gb chunk and switch to it
    
    //char *ptr = kzalloc(4096);
    // this maps physical address 0x1000 to the ptr variable
    //paging_set(paging_4gb_chunk_get_directory(kernel_chunk), (void*)0x1000, (uint32_t)ptr | PAGING_ACCESS_FROM_ALL | PAGING_IS_PRESENT | PAGING_IS_WRITEABLE);

    
    // enable paging
    enable_paging();
   
    /*
    char *ptr2 = (char*)0x1000;
    ptr2[0] = 'A';
    ptr2[1] = 'B';
    print(ptr2);
    print(ptr);
    */

    
    // enable system interripts after idt table is defined, so system does not panic if exception occurs, and instead property calls interrupt
    enable_interrupts();
    char buf[20];
    strcpy(buf, "hello");

    while(1){}
}
