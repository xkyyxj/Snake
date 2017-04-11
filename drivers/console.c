#include "console.h"

static struct cursor cur_posi = {0,0};

void console_init(){
    //cur_posi结构体虽然在程序当中被初始化了，但是初始化为0的话会被编译器认为为没有初始化，放进.bss段当中
    //.bss段不占用外部存储空间，故而在实际访问的时候是访问了.comment段当中的数据，导致错误
    //这是因为当初解析elf格式的kernel的时候是按照Program header当中的内存长度进行拷贝操作的，所以.bss段实际上拷贝了.comment段的数据
    //解决方式：1.解析elf格式文件时初始化.bss段（较复杂）；2.在这里利用代码初始化.bss段（较简单）
    //这里采取了较简单的方式
    cur_posi.x = 0;
    cur_posi.y = 0;
    clear();
}

void clear(){
    u32 start = 0xb8000;
    u32 length = 4000;
    mem_clear(start, length);
}

void put_string(char *str){
    while(*str)
        put_char(*(str++));
}

void put_char(char cha){
    u32 addr_value = 0xb8000 + cur_posi.x * 80 * 2 + cur_posi.y * 2;
    u16 content = 0x0700 | cha;
    u32 *addr = (u32*)addr_value;
    *addr = content;
    next_cursor();
}

/**
 * 当光标超出了显示范围时的换行操作
 *
 */
void out_bound_CRLF() {
    u32 start_addr = 0xb8000 + 80 * 2;
    u32 desi_addr = 0xb8000;
    u32 length = 24 * 80 * 2 / 2;
    asm volatile("rep movsw;"::"S"(start_addr),"D"(desi_addr),"c"(length));

    //清理最后一行对应的显示区域
    mem_clear(0xb8000 + 24 * 80 * 2,80 * 2);

    struct cursor position;
    position.x = 24;
    position.y = 0;
    mov_cursor(position);
}

/**
 * 将光标移动到下一个字符位置（可处理换行）
 *
 */
void next_cursor() {
    struct cursor position;
    if(cur_posi.y < 79) {
        position.x = cur_posi.x;
        position.y = cur_posi.y + 1;
        mov_cursor(position);
    }
    else if(cur_posi.x < 24) {
        position.x = cur_posi.x + 1;
        position.y = 0;
        mov_cursor(position);
    }
    else {
        out_bound_CRLF();
    }
}

/**
 * 将光标移动到指定位置
 *
 * @param position 含有光标的位置信息
 *
 */
bool mov_cursor(struct cursor position) {
    cur_posi.x = position.x;
    cur_posi.y = position.y;
    io_outb(0x3d4,0x0e);
    io_outb(0x3d5, cur_posi.x);
    io_outb(0x3d4,0x0f);
    io_outb(0x3d5,cur_posi.y);
}
