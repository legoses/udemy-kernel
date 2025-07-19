#include "string/string.h"


char tolower(char s1) {
    if(s1 >= 65 && s1 <= 90) {
        s1 += 32;
    }

    return s1;
}


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
* strncpy? WHEN
*/


// read a string until it finds a null terminator, or a terminater specified in the terminator variable
int strnlen_terminator(const char *str, int max, char terminator) {
    int i = 0;
    for(i = 0; i < max; i++) {
        if(str[i] == '\0' || str[i] == terminator) {
            break;
        }
    }

    return i;
}


// case insensitive strncmp
int istrncmp(const char *s1, const char *s2, int n) {
    unsigned char u1, u2;
    while(n-- > 0) {
        u1 = (unsigned char)*s1++;
        u2 = (unsigned char)*s2++;

        if(u1 != u2 && tolower(u1) != tolower(u2))
            return u1 - u2;
        if(u1 == '\0')
            return 0;
    }

    return 0;
}


int strncmp(const char *str1, const char *str2, int n) {
    // here, n will by subtracted after it is used for operation in the while loop
    unsigned char u1, u2;
    while(n-- > 0) {
        //assign character at current address to u1 and u1, then iterate pointer
        u1 = (unsigned char)*str1++;
        u2 = (unsigned char)*str2++;

        if(u1 != u2)
            return u1 - u2;
        if(u1 == '\0')
            return 0;
    }

    return 0;
}


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
