#include "kernel.h"
#include <stdint.h>
#include <stddef.h>
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"

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

size_t strlen(const char *str){
    size_t len = 0;
    while(str[len]){
        len++;
    }
    return len;
}

void print(const char* str){
    size_t len = strlen(str);
    for(int i=0;i<len;++i){
        write_char(str[i],3);
    }
}

void kernel_main()
{
    terminal_initial();
    print("Hello World!\n");

    //Initialize Heap
    kheap_init();

    idt_init();
    
    //enable_interrupts();
    
    void *ptr = kmalloc(50);
    void *ptr1 = kmalloc(5000);
    void *ptr2 = kmalloc(5600);
    kfree(ptr);
    void *ptr4 = kmalloc(50);

    if(ptr || ptr1 ||ptr2 ||ptr4){

    }

}