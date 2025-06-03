#include "streamer.h"
#include "memory/heap/kheap.h"
#include "config.h"


// create a new disk streamer
struct disk_stream *diskstreamer_new(int disk_id) {
    struct disk *disk = disk_get(disk_id);
    if(!disk) {
        return 0;
    }

    struct disk_stream *streamer = kzallox(sizeof(struct disk_stream));
    streamer->pos = 0;
    streamer->disk = disk;

    return streamer;
}


// set the location we are seeking at
int diskstreamer_seek(struct disk_stream *stream, int pos) {
    stream->pos = pos;
    return 0;
}


int diskstreamer_read(struct disk_stream *stream, void *out, int total) {
    int sector = stream->pos / PEACHOS_SECTOR_SIZE; // get the sector number we will be reading from
    int offset = stream->pos % PEACHOS_SECTOR_SIZE; // get of off set from the start of the sector we will be readinf from
    char buf[PEACHOS_SECTOR_SIZE];

    //load the desired sector into the buf
    int res = disk_read_block(stream->disk, sector, 1, buf);
    if(res < 0) {
        goto out;
    }

    //if total is greater than peachos sector size, use peach os sector isze so only a single sector will be read at a time, otherwise use total provided
    int total_to_read = total > PEACHOS_SECTOR_SIZE ? PEACHOS_SECTOR_SIZE : total;

    // loop through the buffer, read into out, while iterating out pointer
    // iterate count is set to what user asked for, so it will not overflow
    for(int i = 0; i < total_to_read; i++) {
        *(char*)out++ = buf[offset+i]
    }


    // adjust the stream if there are further sectors to read
    stream->pos += total_to_read; // update read position
    if(total > PEACHOS_SECTOR_SIZE) {
        res = diskstreamer_read(stream, out, total-PEACHOS_SECTOR_SIZE); // recursive function call if there are more sectors to read
    }

out:
    return res;
}
