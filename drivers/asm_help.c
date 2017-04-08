#include "../include/asm_help.h"

u8 io_inb(u16 port){
    u8 return_value = 0;
    asm volatile("inb %1,%0;":"=a"(return_value):"dN"(port));
    return return_value;
}

void io_outb(u16 port,u8 content){
    asm volatile("outb %1,%0;"::"dN"(port),"aN"(content));
}

u32 io_inl(u16 port){
    u32 return_value = 0;
    asm volatile("inl %1,%0;":"=a"(return_value):"dN"(port));
    return return_value;
}

void io_outl(u16 port, u32 content){
    asm volatile("outl %1,%0;"::"dN"(port),"aN"(content));
}