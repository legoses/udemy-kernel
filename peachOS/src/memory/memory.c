#include "memory.h"

void *memset(void *ptr, int c, size_t size) {
    char *c_ptr = (char*) ptr;
    for (int i = 0; i < size; i++) {
        c_ptr[i] = (char) c;
    }
    return ptr;
}

int memcmp(void *s1, void *s2, int count) {
    char *c1 = s1;
    char *c2 = s2;

    while(count-- > 0) {
        if(*c1++ != *c2++) {
            return c1[-1] < c2[-1] ? -1 : 1; // if memory is not the same, reutrn which has the greater value. -1 means c2 has greater value, 1 means c1 has greater value
        }
    }

    return 0;
}

