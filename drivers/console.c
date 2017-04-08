#include "console.h"

static struct cursor cur_posi = {0,0};

void console_init(){
    clear();
}

void clear(){
    u32 start = 0xb8000;
    u32 length = 4000;
    mem_clear(start, length);
}

void put_string(char *str){
    if(*str)
        put_char(*(str++));
}

void put_char(char cha){
    u32 addr_value = 0xb8000 + cur_posi.x * 80 * 2 + cur_posi.y * 2;
    u16 content = 0x0700 & cha;
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
    struct cursor position;
    position.x = 23;
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