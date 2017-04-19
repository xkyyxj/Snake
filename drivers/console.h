#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "../include/asm_help.h"
#include "../include/m_block.h"

struct cursor {
    u8 x,y;
};

void console_init();

void clear();

void put_string(char *str);

void put_char(char cha);

void next_cursor();

void out_bound_CRLF();

bool mov_cursor(struct cursor position);

void next_line();

#endif