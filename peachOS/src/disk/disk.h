#ifndef DISK_H
#define DISK_H

#include "fs/file.h"

typedef unsigned int PEACHOS_DISK_TYPE;

#define PEACHOS_DISK_TYPE_REAL 0 // represents a real ahrd disk

struct disk {
    PEACHOS_DISK_TYPE type;
    int sector_size;
    struct filesystem *resolve;
};

void disk_search_and_init();
struct disk *disk_get(int index);
int disk_read_block(struct disk *idisk, unsigned int lba, int total, void *buf);

#endif
