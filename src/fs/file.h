#ifndef _FILE_H
#define _FILE_H
#include "path_parser.h"

typedef unsigned int FILE_SEEK_MODE;

enum{
    SEEK_SET,
    SEEK_CUR,
    SEEK_END
};

typedef unsigned int FILE_MODE;

enum{
    FILE_MODE_READ,
    FILE_MODE_WRITE,
    FILE_MODE_APPEND,
    FILE_MODE_INVALID
};

struct disk;
typedef void*(*FS_OPEN_FUNCTION)(struct disk* disk,struct path_part* part,FILE_MODE mode);
typedef int (*FS_RESOLVE_FUNCTION)(struct disk* disk);


struct filesystem{
    FS_RESOLVE_FUNCTION resolve; // if provided disk is using its filesystem return 0
    FS_OPEN_FUNCTION open;

    char name[20];


};

struct file_descriptor{
    int index;
    struct filesystem* filesystem;

    void* private;

    struct disk* disk; //The disk that the file descriptor should be used on.
};

void fs_init();
int fopen(const char* filename,const char* mode);
void fs_insert_filesystem(struct filesystem* filesystem);
struct filesystem* fs_resolve(struct disk* disk);

#endif