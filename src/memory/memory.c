#include "memory.h"

void *memset(void *ptr,int c,size_t size){
    char *c_ptr = (char*) ptr;
    for(int i=0;i<size;++i){
        c_ptr[i] = (char) c;
    }
    return ptr;
}


int memcmp(void* ptr,void* str,int len){
    char *s1 = ptr;
    char *s2 = str;
    

    while(len-- > 0){
        if(*s1++ != *s2++){
            return s1[-1]<s2[-1]?-1:1;
        }
    }

    return 0;
}