#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// page caching refers to something akin to a swap file. Pages that origionate from a secondary storage device like the hard drive or network
#define PAGING_CACHE_DISABLED  0b00010000

// If write through is enalbed, cpu cache  as well as that value in memory is updated atthe same time
// This is simpler and more reliable but more expensive
// Used when there are less write to the cache
//
// Write back only updated the data in the cache and updates in memory at a later time
// Data in meory is only upodated when the cache line is ready to be replaced. Done using least recently used algorithm, LIFO, FIFO
// Dirty bit: each block in cache needs a bit to indicate whether or not the data present the in cache has been modified (dirty) or not modified (clean)
// If a bit is clean, there is not need to write to memory to update
// Designed to redice write to memory. If cache failes or power is lost, the data will be lost
// 
// Paging cache probably needs to be enabled for this bit to matter at all
#define PAGING_WRITE_THROUGH   0b00001000

// allow all rings to access data
#define PAGING_ACCESS_FROM_ALL 0b00000100
#define PAGING_IS_WRITEABLE    0b00000010 // read/ write permissions
#define PAGING_IS_PRESENT      0b00000001 // is the memroy present is memory or does it need ot be loaded in from elsewhere?

#define PAGING_TOTAL_ENTRIES_PER_TABLE 1024
#define PAGING_PAGE_SIZE 4096


// each directory entry managed a 4mb chunk of memory
// each table entry manages a 4kb chunk of memory
// the 10 most significant buts (22-31) represent the index in the directory entry array
// the next 10 bits (12-21) represent the entr in the table array
// This is how the entire 4gb of address space can be addressed with only 20 bits
// a new directory struct can be created for each process
struct paging_4gb_chunk {
    uint32_t *directory_entry;
};

struct paging_4gb_chunk *paging_new_4gb(uint8_t flags);
void paging_switch(uint32_t *directory);
void enable_paging(); // do not call until we have created a 4gb chunk and switched to that directory

uint32_t *paging_4gb_chunk_get_directory(struct paging_4gb_chunk *chunk);

#endif
