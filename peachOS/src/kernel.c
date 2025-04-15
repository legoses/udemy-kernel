#include "kernel.h"
#include <stdint.h>
#include <stddef.h>
#include "idt/idt.h"


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


size_t strlen(const char *str) {
    size_t len = 0;

    while(str[len]) { // while value of str[len] != 0 or '\0'
        len++;
    }

    return len;
}


// write a string to the screen
void print(const char *str) {
    size_t len = strlen(str;

    for(int i = 0; i < len; i++) {
        terminal_writechar(str[i], 15);
    }
}


extern void problem();


void kernel_start() {
    // video_mem[0] = 0x0341; color and ascii code appear reversed because of endianess
    terminal_initialize();
    //video_mem[0] = terminal_make_char('B', 15);
    //terminal_writechar('A', 15);
    //terminal_writechar('B', 15);
    print("Hello, world!\ntest");
    idt_init(); // initialize interrupt descriptor table
    //print("a");
    problem();
}
