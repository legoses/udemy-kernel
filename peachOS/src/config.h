#ifndef CONFIG_H
#define CONFIG_H

#define KERNEL_CODE_SELECTOR 0x08
#define KERNEL_DATA_SELECTOR 0x10

#define PEACHOS_TOTAL_INTERRUPTS 512

// 100MB heap size
#define PEACHOS_HEAP_SIZE_BYTES 104857600
#define PEACHOS_HEAP_BLOCK_SIZE 4096
#define PEACHOS_HEAP_ADDRESS 0x01000000

// 480.5 free kb memory starting at this address. This works because the heap table only takes up 25600 bytes
#define PEACHOS_HEAP_TABLE_ADDRESS 0x00007E00 // This is where the heap table will be stored, not ram.

#define PEACHOS_SECTOR_SIZE 512

#endif
