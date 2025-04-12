#include "kernel.h"


uint16_t *video_mem = 0;

uint16_t terminal_make_char(char c, char color) {
    return (color << 8) | c;
}

// initialize terminal to get rid of sea bios output
void terminal_initialize() {
    video_mem = (uint16_t*)(0xB8000); // create character array that is stored at this address in order to print characters to the screen

    //convert x and y coords to store in 1 dimensional array
    for(int y = 0; y < VGA_HEIGHT; y++) {
        for(int x = 0; x < VGA_WIDTH; x++) {
            video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(' ', 0); // store spaces in array
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

void kernel_start() {
    // video_mem[0] = 0x0341; color and ascii code appear reversed because of endianess
    terminal_initialize();
    video_mem[0] = terminal_make_char('B', 15);

}
