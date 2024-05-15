#include "stream.h"
#include "memory/heap/kheap.h"
#include "config.h"

struct stream* stream_new(int disk_id){
    struct disk* disk = disk_get(disk_id);
    if(!disk){
        return 0;
    }

    struct stream* streamer = kzalloc(sizeof(struct stream));
    streamer->byte_pos = 0;
    streamer->disk = disk;

    return streamer;
}

int streamer_seek(struct stream* streamer,int pos){
    streamer->byte_pos = pos;
    return 0;
}

int streamer_read(struct stream* stream,void* out,int total){
    int sector = stream->byte_pos / SECTOR_SIZE;
    int offset = stream->byte_pos % SECTOR_SIZE;
    char buf[SECTOR_SIZE];

    int res = disk_read_block(stream->disk,sector,1,buf);
    if(res < 0){
        goto out;
    }

    int total_to_read = (total > SECTOR_SIZE)?SECTOR_SIZE:total;
    for(int i=0;i<total_to_read;++i){
        *(char*)out++ = buf[offset+i];
    }
    
    //Record the new position of disk;
    stream->byte_pos += total_to_read;
    if(total > SECTOR_SIZE){
        res = streamer_read(stream,out,total-SECTOR_SIZE);
    }
    out:
        return res;
}

void streamer_free(struct stream* streamer){
    kfree(streamer);
}