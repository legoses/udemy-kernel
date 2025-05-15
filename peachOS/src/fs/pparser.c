#include "fs/pparser.h"
#include "kernel.h"
#include "string/string.h"
#include "memory/heap/kheap.h"
#include "memory/memory.h"

static int path_valid_format(const char *filename) { // does not support relative paths for now
    int len = strnlen(filename, PEACHOS_MAX_PATH);

    // makes sure path is proper length, that is starts with a drive number, and that is begins with path specifier ':/'
    return (len >= 3 && isdigit(filename[0]) && memcmp((void*) &filename[1], ":/", 2) == 0);
}
