#ifndef ARCH_X86_MEMORY_H_
#define ARCH_X86_MEMORY_H_

#include "../../../../include/type.h"

struct addr_range_desc{
    u32 base_addr_low;
    u32 base_addr_high;
    u32 length_low;
    u32 length_high;
    u32 type;
};

u32 get_free_page();

#endif
