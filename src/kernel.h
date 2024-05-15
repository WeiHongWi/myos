#ifndef KERNEL_H
#define KERNEL_H

#define VAR_WIDTH  80
#define VAR_HEIGHT   20

#define MAX_PATH_LEN 108

void kernel_main();
void print(const char* str);
void idt_init();
#endif