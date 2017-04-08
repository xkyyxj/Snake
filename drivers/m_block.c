#include "../include/m_block.h"

void mem_clear(u32 start, u32 length){
    asm volatile("cld\n\t"
                 "rep stosl":
                 :"a"(0),"c"(length),"D"(start));
}

void k_memcpy(u32 start, u32 entry, u32 length){
    //TODO 内存拷贝函数
}