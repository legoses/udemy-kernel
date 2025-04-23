#include "heap.h"
#include "kernel.h"
#include "status.h"
#include <stdbool.h>

// make sure address provided align to a 4096 address
static bool heap_validate_alignment(void *ptr) {
    return ((unsigned int) ptr % PEACHOS_HEAP_BLOCK_SIZE) == 0;
}

// must pass an uninitialized heap, pass where you want heap to start, where you want heap to end, and a valid and filled in heap table
int heap_create(struct heap *heap, void *ptr, void *end, struct heap_table *table) {
    int res = 0;

    if(!heap_validate_alignment(ptr) || !heap_validate_alignment(end)) {
        res = -EINVARG;
        goto out;
    }

out:
    return res;
}
