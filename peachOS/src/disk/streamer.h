#ifndef STREAMER_H
#define STREAMER_H

#include "disk.h"

struct disk_stream {
    int pos; // represents our position we are seeking at in the disk stream
    struct disk* disk;
}

#endif
