#include "io/io.h"

int disk_read_sector(int lba, int total, void *buf) {
    // outb writes a single byte to the specified port
    // LBA stands for linear block address. It is a block of storatge (512 bytes per sector is prety standard) that is linearly addressed either relative to the start of the disk, or 
    // the start of a partition or file system
    outb(0x1F6, (lba >> 24) | 0xE0); // standby immediate
    outb(0x1F2, total);
    outb(0x1F3, (unsigned char)(lba & 0xff));
    outb(0x1F4, (unsigned char)(lba >> 8));
    outb(0x1F5, (unsigned char)(lba >> 16));
    outb(0x1F7, 0x20); // read sectors command
    
    unsigned short *ptr = (unsigned short*) buf;

    for(int b = 0; b < total; b++) {
        // wait for the buffer to be ready
    }

    return 0;
}
