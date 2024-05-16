#include "file.h"
#include "config.h"
#include "memory/memory.h"
#include "memory/heap/kheap.h"
#include "status.h"
#include "kernel.h"

struct filesystem* filesystems[MAX_NUM_FILESYSTEM];
struct file_descriptor* file_descriptor[MAX_NUM_FILE_DESCRIPTOR];


static struct filesystem** fs_get_free_filesystem(){
    int i = 0;
    for(i = 0;i < MAX_NUM_FILESYSTEM ;++i){
        if(filesystems[i] == 0){
            return &filesystems[i];
        }
    }
    return 0;
}

void fs_insert_filesystem(struct filesystem* filesystem){
    struct filesystem** fs;
    fs = fs_get_free_filesystem();
    if(!fs){
        print("Insert filesystem waited\n");
        while(1){}
    }
    *fs = filesystem;
}

static void fs_static_load(){
    //fs_insert_filesystem(fat16_init());
}

void fs_load(){
    memset(filesystems, 0 ,sizeof(filesystems));
    fs_static_load();
}

void fs_init(){
    memset(file_descriptor,0,sizeof(file_descriptor));
    fs_load();
}

static int file_new_descriptor(struct file_descriptor** desc_out){
    int res = -ENOMEM;

    //Find the empty file descriptor id
    for(int i = 0;i<MAX_NUM_FILE_DESCRIPTOR;++i){
        if(file_descriptor[i] == 0){
            struct file_descriptor* tmp = kzalloc(sizeof(struct file_descriptor));
            tmp->index = i+1;
            file_descriptor[i] = tmp;
            *desc_out = tmp;
            res = 0;
            break;
        }
    }

    return res;
}

static struct file_descriptor* file_get_descriptor(int fd){
    if( fd <= 0 || fd>= MAX_NUM_FILE_DESCRIPTOR){
        return 0;
    }
    int index = fd + 1;
    return file_descriptor[index];
}

struct filesystem* fs_resolve(struct disk* disk){
    struct filesystem* fs = 0;

    for(int i=0; i<MAX_NUM_FILESYSTEM ;++i){
        if(filesystems[i] != 0 && filesystems[i]->resolve(disk) == 0){
            fs = filesystems[i];
            break;
        }
    }

    return fs;
}

int fopen(const char* filename,const char* mode){
    return -EIO;
}