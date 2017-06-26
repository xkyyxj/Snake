#ifndef DISK_DRIVERS_H_
#define DISK_DRIVERS_H_

#define DISK_WRITE 0
#define DISK_READ 1

#define PER_SECTION_BYTE 512

#include "../include/type.h"
#include "../arch/x86/kernel/traps.h"

void init_disk_int();

void disk_write(u8 *content,s32 length);

u8* disk_read(u32 length);

void write_one_disk(u8 *content);

//------------------中断态运行------------------
void disk_interrupt();

#endif

