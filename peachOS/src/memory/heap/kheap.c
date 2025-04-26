#include "kheap.h"
#include "heap.h"
#include "config.h"
#include "kernel.h"

struct heap kernel_heap;
struct heap_table kernel_heap_table;

/*
    * We will be allocating 100mb of ram
    * 1. Get number of bytes in a megabyte: 1024 * 1024 = 1048576
    * 2. Multiply by 100 to get total bytes: 100 * 1048576 = 104857600
    * 3. Divide by block size (4096) to get total number of blocks: 25600
*/
void kheap_init() {
    int total_table_entries = PEACHOS_HEAP_SIZE_BYTES / PEACHOS_HEAP_BLOCK_SIZE;
    kernel_heap_table.entries = (HEAP_BLOCK_TABLE_ENTRY*)(PEACHOS_HEAP_TABLE_ADDRESS);
    kernel_heap_table.total = total_table_entries;

    void *end = (void*)(PEACHOS_HEAP_ADDRESS + PEACHOS_HEAP_SIZE_BYTES); // point to address of the end of table
    int res = heap_create(&kernel_heap, (void*)(PEACHOS_HEAP_ADDRESS), end, &kernel_heap_table);

    if(res < 0) {
        print("Failed to create heap\n");
    }
}


void *kmalloc(size_t size) {
    return heap_malloc(&kernel_heap, size);
}


void kfree(void *ptr) {
    heap_free(&kernel_heap, ptr);
}
