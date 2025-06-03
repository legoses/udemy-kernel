#include "io/io.h"
#include "disk/disk.h"
#include "config.h"
#include "memory/memory.h"
#include "status.h"

struct disk disk;

int disk_read_sector(int lba, int total, void *buf) {
    // outb writes a single byte to the specified port
    // LBA stands for linear block address. It is a block of storatge (512 bytes per sector is prety standard) that is linearly addressed either relative to the start of the disk, or 
    // the start of a partition or file system
    outb(0x1F6, (lba >> 24) | 0xE0); // standby immediate. 0x1F6 is used to select a drive to use
    outb(0x1F2, total); // load length into sector count register
    outb(0x1F3, (unsigned char)(lba & 0xff)); // load low address to read/ write here
    outb(0x1F4, (unsigned char)(lba >> 8)); //load mif address to read /write here
    outb(0x1F5, (unsigned char)(lba >> 16)); // load higfh address to read/ write here
    outb(0x1F7, 0x20); // send ATA command to device
    
    unsigned short *ptr = (unsigned short*) buf; // read two byte  at a time

    for(int b = 0; b < total; b++) {
        // wait for the buffer to be ready
        // read status from port at 0x1F7
        char c = insb(0x1F7);

        while(!(c & 0x08)) {
            c = insb(0x1F7);
        }

        // copy from hard disk to memory
        for(int i = 0; i < 256; i++) {
            *ptr = insw(0x1F0);
            ptr++;
        }
    }

    return 0;
}


// abstract setting up a disk so it is easier to add support for more than one disk later
void disk_search_and_init() {
     memset(&disk, 0, sizeof(disk));
    disk.type = PEACHOS_DISK_TYPE_REAL;
    disk.sector_size = PEACHOS_SECTOR_SIZE;
}


struct disk *disk_get(int index) {
    if(index != 0) {
        return 0;
    }

    return &disk;
}


// abstract reading from the disk into a buffer, make sure we are reading from the correct disk
// buffer is void because we are reading unstructured data and casting it later
// reads an entore secor at a time
int disk_read_block(struct disk *idisk, unsigned int lba, int total, void *buf) {
    if(idisk != &disk) {
        return -EIO;
    }

    return disk_read_sector(lba, total, buf);
}
