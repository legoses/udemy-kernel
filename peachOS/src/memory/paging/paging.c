#include "paging.h"
#include "memory/heap/kheap.h"
#include "status.h"


// because of how tabels are aligned in memory, only 21 bits are used for the address, the rest are flags
// the most signifigant 10 bits (22-31) (little endian bytes, so right most bit is most signifigant) represent the index in the page directory array, the next 10 bits (12-21) specify the index of the entry in the page table array


void paging_load_directory(uint32_t *directory); // assembly function


static uint32_t *current_directory = 0;


struct paging_4gb_chunk *paging_new_4gb(uint8_t flags) {
    uint32_t *directory = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE); // create paging directory

    int offset = 0;

    for(int i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; i++) {
        uint32_t *entry = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE); // allocate page table entries in a linear pashion

        for(int b = 0; b < PAGING_TOTAL_ENTRIES_PER_TABLE; b++) {
            entry[b] = (offset + (b * PAGING_PAGE_SIZE)) | flags;
        }
        offset += (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE);
        directory[i] = (uint32_t) entry | flags | PAGING_IS_WRITEABLE; // entire directory entry is wrigable. Insividual pages may be set to non-writable
        
    }
    struct paging_4gb_chunk *chunk_4gb = kzalloc(sizeof(struct paging_4gb_chunk));
    chunk_4gb->directory_entry = directory;

    return chunk_4gb;
}


void paging_switch(uint32_t *directory) {
    paging_load_directory(directory);
    current_directory = directory;
}


uint32_t *paging_4gb_chunk_get_directory(struct paging_4gb_chunk *chunk) { // extract directory entry
    return chunk->directory_entry;
}


// make sure address is aligned with 4096 chunk of memory
bool paging_is_aligned(void *addr) {
    return ((uint32_t) addr % PAGING_PAGE_SIZE) == 0;
}


// take virtual address and get its directory entry and table index
int paging_get_indexes(void *virtual_address, uint32_t *directory_index_out, uint32_t *table_index_out) {
    int res = 0;

    if(!paging_is_aligned(virtual_address)) { 
        res = -EINVARG;
        goto out;
    }
   
    // set the address that directory_index_out points to to the value that is the address of the directory index, instead of setting dierctory_index_out to point to that address
    // this is because we want the actual value of the address, not the contents that are stored at this address
    *directory_index_out = ((uint32_t) virtual_address / (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE)); 

    //get memory off set from directory entry, then divide by page size to convert to index number
    *table_index_out = ((uint32_t) virtual_address % (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE) / PAGING_PAGE_SIZE);

out:
    return res;
}
