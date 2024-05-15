#include "path_parser.h"
#include "kernel.h"
#include "string/string.h"
#include "memory/heap/kheap.h"
#include "memory/memory.h"
#include "status.h"

static int path_valid_format(const char* filename){
    int len = t_strlen(filename,MAX_PATH_LEN);
    return (len >= 3 && isdigit(filename[0]) && memcmp( (void*) &filename[1],":/",2) == 0);
}

static int pathparser_get_drive_by_path(const char** path){
    if(!path_valid_format(*path)){
        return -EBADPATH;
    }
    
    //Record drive number 
    int drive_no = char_to_numeric(*path[0]);

    //Skip disk drive number , ex : C:/ ,D:/ , ...
    *path += 3;

    return drive_no;
}

static struct path_root* pathparser_create_root(int drive_number){
    struct path_root* root = kzalloc(sizeof(struct path_root*));
    root->drive_no = drive_number;
    root->first = 0;

    return root;
}

static const char* pathparser_get_path_part(const char** path){
    char* result_path_part = kzalloc(MAX_PATH_LEN);
    int i = 0;
    while( *(*path) != '/' && *(*path) != 0x00){
        result_path_part[i] = *(*path);
        *path+=1;
        i++;
    }
    
    if (**path == '/'){
        *path+=1;
    }

    if( i == 0){
        kfree(result_path_part);
        result_path_part = 0;
    }

    return result_path_part;
}

struct path_part* pathparser_set_path_part(struct path_part* last,const char** path){

    const char* path_part_str = pathparser_get_path_part(path);
    if(!path_part_str){
        return 0;
    }

    struct path_part* part = kzalloc(sizeof(struct path_part));
    part->part = path_part_str;
    part->next = 0x00;

    if(last){
        last->next = part;
    }

    return part;
}

void pathparser_free(struct path_root* root){
    struct path_part* part = root->first;
    while(part){
        struct path_part* tmp = part->next;
        
        kfree((void*) part->part);
        kfree(part);
        
        part = tmp;
    }
    kfree(root);
}

struct path_root* pathparser_parse(const char* path,const char* current_directory_path){
    int res = 0;
    const char* tmp = path;
    struct path_root* root = 0;

    if(strlen(path) > MAX_PATH_LEN){
        goto out;
    }
    
    res = pathparser_get_drive_by_path(&tmp);
    if(res < 0){
        goto out;
    }

    root = pathparser_create_root(res);
    if(!root){
        goto out;
    }

    struct path_part* first_part = pathparser_set_path_part(NULL,&tmp);
    if(!first_part){
        goto out;
    }

    root->first = first_part;
    struct path_part* p = pathparser_set_path_part(first_part,&tmp);
    while(p){
        p = pathparser_set_path_part(p,&tmp);
    }

    out:
        return root;
}