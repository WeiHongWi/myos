#include "heap.h"
#include "kernel.h"
#include "status.h"
#include "memory/memory.h"

static int heap_validate_table(void *ptr,void *end,struct heap_table* table){
    int res = 0;

    size_t table_size = (size_t)(end - ptr);
    size_t total_num_block = table_size / HEAP_BLOCK_SIZE;

    if(table->total != total_num_block){
        res = -EINVARG;
        goto out;
    }
    out:
        return res;
}

static int heap_valid_alignment(void* ptr){
    return ((unsigned int) ptr % HEAP_BLOCK_SIZE) == 0;
}

int heap_create(struct heap* heap,void *ptr,void *end,struct heap_table* table){
    int res = 0;

    if( !heap_valid_alignment(ptr) || !(heap_valid_alignment(end)) ){
        res = -EINVARG;
        goto out;
    }
    memset(heap,0,sizeof(struct heap));
    heap->addr = ptr;
    heap->table = table;
    
    res = heap_validate_table(ptr,end,table);
    if(res < 0){
        goto out;
    }

    size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY) * table->total;
    memset(table->entries,HEAP_BLOCK_TABLE_ENTRY_FREE,table_size);
    out:
        return res;
}


static uint32_t align_block_num(uint32_t value){
    if ((value % HEAP_BLOCK_SIZE) == 0){
        return value;
    }
    value = value - (value % HEAP_BLOCK_SIZE);
    value += HEAP_BLOCK_SIZE;

    return value;
}
int heap_get_start_block(struct heap* heap,size_t total_block){
    struct heap_table* table = heap->table;
    int block_curr = 0;
    int block_start = -1;

    for(size_t i=0;i<table->total;++i){
        if( (table->entries[i] & 0x0f)  != HEAP_BLOCK_TABLE_ENTRY_FREE){
            block_curr = 0;
            block_start = -1;
            continue;
        }

        if(block_start == -1){
            block_start = i;
        }
        block_curr++;
        if(block_curr == total_block){
            break;
        }
    }

    if(block_start == -1){
        return -ENOMEM;
    }

    return block_start;
}

void* block_to_address(struct heap* heap,size_t total_block){
    return heap->addr + (total_block * HEAP_BLOCK_SIZE);
}

void block_mark_taken(struct heap* heap,int start_block,int total_block){
    int end_block = (total_block + start_block) - 1;

    //First block initialization
    HEAP_BLOCK_TABLE_ENTRY entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST;
    if(total_block > 1){
        entry |= HEAP_BLOCK_HAS_NEXT;
    }
    struct heap_table* table= heap->table;

    for(int i=start_block;i<=end_block;++i){
        table->entries[i] = entry;
        entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN;

        if(i!=end_block-1){
            entry |= HEAP_BLOCK_HAS_NEXT;
        }
    }
}
void* heap_malloc_block(struct heap* heap ,size_t total_block){
    void* address = 0;

    int start_block = heap_get_start_block(heap,total_block);
    if(start_block < 0){
        goto out;
    }
    
    address = block_to_address(heap,start_block);
    block_mark_taken(heap,start_block,total_block);
    out:
        return address;
}
void* heap_malloc(struct heap* heap ,size_t sz){
    size_t aligned_size = align_block_num(sz);
    uint32_t total_blocks = aligned_size / HEAP_BLOCK_SIZE;

    return heap_malloc_block(heap,total_blocks);
}

int address_to_block(struct heap* heap,void* ptr){

    return ((int)(ptr-heap->addr))/HEAP_BLOCK_SIZE;
}

void block_mark_free(struct heap* heap,int block){
    struct heap_table* table = heap->table;
    for(int i=block;i<(int)table->total;++i){
        HEAP_BLOCK_TABLE_ENTRY entry = table->entries[i];
        table->entries[i] = HEAP_BLOCK_TABLE_ENTRY_FREE;

        if(!(entry & HEAP_BLOCK_HAS_NEXT)){
            break;
        }
    }
}
void heap_free(struct heap* heap,void *ptr){
    block_mark_free(heap,address_to_block(heap,ptr));
}