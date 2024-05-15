#ifndef STREAM_H
#define STREAM_H

#include "disk.h"

struct stream{
    int byte_pos;
    struct disk* disk;
};
struct stream* stream_new(int disk_id);
int streamer_seek(struct stream* streamer,int pos);
int streamer_read(struct stream* stream,void* out,int total);
void streamer_free(struct stream* streamer);

#endif