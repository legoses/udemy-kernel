#ifndef STREAMER_H
#define STREAMER_H

#include "disk.h"

struct disk_stream {
    int pos; // represents our position we are seeking at in the disk stream
    struct disk* disk;
};

struct disk_stream *diskstreamer_new(int disk_id);
int diskstreamer_read(struct disk_stream *stream, void *out, int total);
int diskstreamer_seek(struct disk_stream *stream, int pos);
void diskstream_close(struct disk_stream *stream);

#endif
