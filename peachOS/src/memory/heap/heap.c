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
    return val;
}


// HEAP_BLOCK_TABLE_ENTRY is just a typecasted unsigned char
static int heap_get_entry_type(HEAP_BLOCK_TABLE_ENTRY entry) {
    return entry & 0b11110000; // get lower 4 bits to determine memory block type
}


int heap_get_start_block(struct heap *heap, uint32_t total_blocks) {
    struct heap_table *table = heap->table;
    int bc = 0; // store current block
    int bs = -1; // store first block we find that is free

    // loop through entire table to find free blocks
    for(size_t i = 0; i < table->total; i++) {
        if(heap_get_entry_type(table->entries[i]) != HEAP_BLOCK_TABLE_ENTRY_FREE) { // if non free block is found, restart bs and bc
            bc = 0; bs = -1;
            continue; // go back to start of loop
        }

        if(bs == -1) {
            bs = i; // get start block
        }

        bc++;

        if(bc == total_blocks) {
            break;
        }
    }

    if(bs == -1) {
        return -ENOMEM; // if bs is still -1, reutrn no memory error
    }

    return bs; // return starting block number

}


void *heap_block_to_address(struct heap *heap, int block) {
    return heap->saddr + (block * PEACHOS_HEAP_BLOCK_SIZE); // calculate offset from heap start. start + (offset * block_size)
}


void heap_mark_blocks_taken(struct heap *heap, int start_block, int total_blocks) {
    int end_block = (start_block + total_blocks) - 1;

    HEAP_BLOCK_TABLE_ENTRY entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST;
    if(total_blocks > 1) {
        entry |= HEAP_BLOCK_HAS_NEXT; // indicate this is not the last block
    }

    for(int i = start_block; i <= end_block; i++) { // loop through every block being allocated
        heap->table->entries[i] = entry; // if first entry, this will have HEAP_BLOCK_IS_FIRST flag set
        entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN; // remove first entry flag

        if(i != end_block - 1) {
            entry |= HEAP_BLOCK_HAS_NEXT; // set next block flag if not last block
        }
    }
}


void *heap_malloc_blocks(struct heap *heap, uint32_t total_blocks) {
    void *address = 0;

    int start_block = heap_get_start_block(heap, total_blocks); // get the start block of memory being allocated
    
    if(start_block < 0) {
        goto out;
    }

    address = heap_block_to_address(heap, start_block); // calculate abolsute address of beginnig block

    // mark block as taken
    heap_mark_blocks_taken(heap, start_block, total_blocks);

out:
    return address;
}


void *heap_malloc(struct heap *heap, size_t size) {
    size_t aligned_size = heap_align_value_to_upper(size); // calculate size to allocate
    uint32_t total_blocks = aligned_size / PEACHOS_HEAP_BLOCK_SIZE; //calculate how many blocks to allocate
    return heap_malloc_blocks(heap, total_blocks);
}


int heap_address_to_block(struct heap *heap, void *address) {
    return ((int)(address - heap->saddr)) / PEACHOS_HEAP_BLOCK_SIZE; // convert from memroy address to table block number
}


void heap_mark_blocks_free(struct heap *heap, int starting_block) {
    struct heap_table *table = heap->table;
    for(int i = starting_block; i < (int)table->total; i++) {
        HEAP_BLOCK_TABLE_ENTRY entry = table->entries[i];
        table->entries[i] = HEAP_BLOCK_TABLE_ENTRY_FREE; // free memory to it can be reused by malloc
        
        if(!(entry & HEAP_BLOCK_HAS_NEXT)) {
            break;
        }
        
    }
}


void heap_free(struct heap *heap, void *ptr) {
    heap_mark_blocks_free(heap, heap_address_to_block(heap, ptr)); // maybe add bounds check to make sure ptr is valid later
}
