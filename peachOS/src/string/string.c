#include "string/string.h"

int strlen(const char *ptr) {
    int i = 0;
    while(*ptr != 0) {
        i++;
        ptr += 1;
    }

    return i;
}

int strnlen(const char *ptr, int max) {
    int i = 0;
    for(i = 0; i < max; i++) {
        if(ptr[i] == 0) {
            break;
        }
    }

    return i;
}


bool isdigit(char c) {
    return c >= 48 && c <= 57;
}

/*
* strncpy?
*/

char *strcpy(char *dest, const char *src) {
    char *res = dest;
    while(*src != 0) {
        *dest = *src; // set equal to value stored at current location pointed to by src
        src += 1; // increase pointer counter for these two
        dest += 1;
    }

    *dest = 0x00; // null terminator for string

    return res;
}


int tonumericdigit(char c) {
    return c - 48;
}
