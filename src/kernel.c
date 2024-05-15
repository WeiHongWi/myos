#include "kernel.h"
#include <stdint.h>
#include <stddef.h>
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "disk/disk.h"
#include "fs/path_parser.h"
#include "string/string.h"
#include "disk/stream.h"

uint16_t *video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

uint16_t set_char(char c , char color){
    return (color<<8)|c;
}
void put_char(int x,int y,char c,char color){
    video_mem[(y*VAR_WIDTH)+x] = set_char(c,color);
}

void write_char(char c,char color){
    if(c == '\n'){
        terminal_row += 1;
        terminal_col = 0;
        return;
    }
    put_char(terminal_col,terminal_row,c,color);
    terminal_col+=1;
    if(terminal_col >= VAR_WIDTH){
        terminal_col = 0;
        terminal_row += 1;
    }
}
void terminal_initial(){
    video_mem = (uint16_t*) 0xB8000; 
    for(int y=0;y<VAR_HEIGHT;++y){
        for(int x=0;x<VAR_WIDTH;++x){
            put_char(x,y,' ',0);
        }
    }
}

void print(const char* str){
    size_t len = strlen(str);
    for(int i=0;i<len;++i){
        write_char(str[i],3);
    }
}

static struct paging_4GB_chunk* kernel_chunk = 0;
void kernel_main()
{
    terminal_initial();
    print("Hello World!\n");

    //Initialize Heap
    kheap_init();
    
    //Intialize disk
    disk_search_and_init();

    //Initialize interrupt description table
    idt_init();
    
    //set up paging
    kernel_chunk = paging_4gb_init(PAGING_IS_WRITEABLE | PAGING_ACCESS_FROM_ALL | PAGING_IS_PRESENT);
    paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));
    
    //Enable paging
    enable_paging();

    //enable_interrupts();
    
    //struct path_root* root = pathparser_parse("0:/bin/shell.exe",NULL);

    //if(root){

    //}
    struct stream* streamer = stream_new(0);
    streamer_seek(streamer,0x201);
    unsigned char c = 0;
    streamer_read(streamer,&c,1);
    while(1){}

}