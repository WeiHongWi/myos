#include "string.h"

int strlen(const char* filename){
    int len = 0;
    char *ptr = (char *)filename;
    
    while(*ptr != '\0'){
        len++;
        ptr++;
    }
    
    return len;
}

int t_strlen(const char* filename ,int max){
    int len = 0;
    for(len = 0;len < max;++len){
        if(filename[len] == 0){
            break;
        }
    }

    return len;
}
bool isdigit(char c){
     return c>=48 && c<=57;
}

int char_to_numeric(char c){
    return c-48;
}

char* strcpy(char* dest,const char* src){
    char *tmp = dest;
    while(*src != 0){
        *dest = *src;
        src += 1;
        dest += 1;
    }
    
    *dest = 0x00;
    
    return tmp;
}