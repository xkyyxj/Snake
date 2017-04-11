#include "apic.h"

static volatile u32* eoi_reg_addr = (u32*)0xfee000b0;

void io_apic_eoi(){
    *eoi_reg_addr = 1;
}
