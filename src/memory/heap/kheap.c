#include "kheap.h"
#include "heap.h"
#include "kernel.h"
#include "memory/memory.h"

struct heap kernel_heap;
struct heap_table kernel_heap_table;

void kheap_init(){
    int total_entries = HEAP_SIZE_BYTES / HEAP_BLOCK_SIZE;
    kernel_heap_table.entries = (HEAP_BLOCK_TABLE_ENTRY*)(HEAP_TABLE_ADDRESS);
    kernel_heap_table.total = total_entries;

    void *end = (void*)(HEAP_ADDRESS + HEAP_SIZE_BYTES);
    int res = heap_create(&kernel_heap,(void*)HEAP_ADDRESS,end,&kernel_heap_table);

    if(res<0){
        print("Failed to allocate\n");
    }

}


void* kmalloc(size_t size){
     return heap_malloc(&kernel_heap,size);
}

void* kzalloc(size_t size){
    void* ptr = kmalloc(size);
    if(!ptr){
        return 0;
    }
    memset(ptr,0,size);
    return ptr;
}

void kfree(void* ptr){
    heap_free(&kernel_heap,ptr);

}