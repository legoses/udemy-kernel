#include "heap.h"
#include "kernel.h"
#include "status.h"
#include "memory/memory.h"
#include <stdbool.h>

static int heap_validate_table(void *ptr, void *end, struct heap_table *table) { // make sure make sure heap table size agrees with memory we calculated, and that it is aligned with 4096
    int res = 0;
    
    size_t table_size = (size_t)(end - ptr);
    size_t total_blocks = table_size / PEACHOS_HEAP_BLOCK_SIZE;

    if(table->total != total_blocks) {
        res = -EINVARG;
        goto out;
    }

out:
    return res;
}

// make sure address provided align to a 4096 address
static bool heap_validate_alignment(void *ptr) {
    return ((unsigned int) ptr % PEACHOS_HEAP_BLOCK_SIZE) == 0;
}

// must pass an uninitialized heap, pass where you want heap to start, where you want heap to end, and a valid and filled in heap table
int heap_create(struct heap *heap, void *ptr, void *end, struct heap_table *table) {
    int res = 0;

    if(!heap_validate_alignment(ptr) || !heap_validate_alignment(end)) {
        res = -EINVARG;
        goto out; // I guess goto isnt bad in c?
    }

    memset(heap, 0, sizeof(struct heap));
    heap->saddr = ptr; //pointer to start of heap
    heap->table = table; // pointer to memory table. Currently being set to addr 0x00007E00
    
    res = heap_validate_table(ptr, end, table);
    
    if(res < 0) {
        goto out;
    }

    size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY) * table->total; // zero out memory where heap table will be stored so no falgs are accidently set
    memset(table->entries, HEAP_BLOCK_TABLE_ENTRY_FREE, table_size);

out:
    return res;
}

// if process requests memory size smaller than our block size, convert the value to a size aligned with 4096 block size
static uint32_t heap_align_value_to_upper(uint32_t val) { 
    if((val % PEACHOS_HEAP_BLOCK_SIZE) == 0) {
        return val;
    }

    val = (val - (val % PEACHOS_HEAP_BLOCK_SIZE)); // align thair value with the lower block aligned size
    val += PEACHOS_HEAP_BLOCK_SIZE; // add 4096 to val
    reutrn val;
}


void *heap_malloc(size_t size) {
    return 0;
}


void heap_free(void *ptr) {
    return 0;
}
