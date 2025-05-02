#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>

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

struct paging_4gb_chunk {
    uint23_t *directory_entry;
};

#endif
