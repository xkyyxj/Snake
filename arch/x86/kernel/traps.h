#ifndef ARCH_X86_KERNEL_TRAPS_H_
#define ARCH_X86_KERNEL_TRAPS_H_

#include "../../../include/type.h"

#define IDT_START 0 //IDT起始地址

typedef void (*INT_FUNC)();

//中断向量表当中的中断条目
struct __attribute__((packed)) int_descriptor{
    u16 low_addr;
    u16 descriptor;
    u16 attribute;
    u16 high_addr;
};

void trap_init();

void set_trap_gate(u8 num,INT_FUNC func);

void empty_int();

/*
 * 开启中断
 *
 * @pram int_num 中断号
 */
void open_trap(u8 int_num);

#endif
