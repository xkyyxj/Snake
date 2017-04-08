#ifndef INCLUDE_ASM_HELP_H_
#define INCLUDE_ASM_HELP_H_

#include "type.h"

u8 io_inb(u16 port);

void io_outb(u16 port,u8 content);

u32 io_inl(u16 port);

void io_outl(u16 port, u32 content);

#endif