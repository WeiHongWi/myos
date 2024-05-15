## Implement the core function of operating system
### Booting
* Linker
* Segment Initialization
* Real mode to Protected mode
* Hello world!

### [Interrupt](<https://github.com/WeiHongWi/myos/tree/main/src/idt>)
* Implement the interrupt description table and interrupt

### [Heap](<https://github.com/WeiHongWi/myos/tree/main/src/memory/heap>)
* Implement heap with basic function : malloc and free
* Improve fragmentation
* Implement Garbage Collection to avoid forgetting free the memory

### [Paging](<https://github.com/WeiHongWi/myos/tree/main/src/memory/paging>)
* Implement page directory and page table
* Mapping the virtual address to physical address by paging

### [File System](<https://github.com/WeiHongWi/myos/tree/main/src/fs>)
* Implement path parset by linked list
