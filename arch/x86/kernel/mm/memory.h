#ifndef ARCH_X86_MEMORY_H_
#define ARCH_X86_MEMORY_H_

#include "../../../../include/type.h"

#include "../../../../drivers/Keyboard.h"

#define ADDRESS_RANGE_FREE 1
#define ADDRESS_RANGE_RESERVED 2

#define MEM_ENTRY_NUM 6

#define IN_USE 0
#define IN_FREE 1

#define ALL_USE 0
#define ONE_USE 1
#define ALL_FREE 2

struct addr_range_desc{
    u32 base_addr_low;
    u32 base_addr_high;
    u32 length_low;
    u32 length_high;
	u32 type;
};

struct memory_info_entry{
	u32 base_addr_low;
	u32 base_addr_high;
	u32 length_low;
	u32 length_high;
	u8 mem_type;
	struct memory_info_entry *next,*previouse;
};

void print_memory();

u32 get_free_page();

u32 vmalloc(u32 size);

u32 pmalloc(u32 size);

u32 vfree(u32 base_addr);

bool pfree(u32 base_addr);

void init_mem_info();

void print_memory();

#endif
