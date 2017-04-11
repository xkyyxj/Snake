#include "traps.h"

//对于内核采用平坦模式，并且内核的代码段总是GDT当中的第一个空而用描述符
//因此，中断向量表项当中的代码段选择子总是GDT当中第一个可用描述符
void set_trap_gate(u8 num,INT_FUNC func){
    struct int_descriptor *start = (struct int_descriptor*)IDT_START;
    u32 func_posi = (u32)func;
    start += num;
    u16 low_addr = func_posi & 0xffff;
    u16 high_addr = (func_posi >> 16) & 0xffff;
    start->low_addr = low_addr;
    start->high_addr = high_addr;

    //内核代码都应该位于GDT的第二项描述符当中
    start->descriptor = 0x0008;

    //中断描述的属性（第三个字）
    //DPL = 0
    start->attribute = 0x8e00;
}

struct idtr_struct{
        u16 length;
        u32 base_addr;
} __attribute__((packed));

//主要应该做的事情是初始化保护模式下的中断向量表
//决定将保护模式下的中断向量表设置的内存的最开始0字节处
void trap_init(){
    //下面是暂时将可用中断都设置为空中断
    //TODO-实际上对于Intel保留的中断等应该在此处进行设置
    for(int i = 0;i < 255;i++){
        set_trap_gate(i,&empty_int);
    }
    //初始化中断向量表寄存器IDTR
    struct idtr_struct idtr;
    idtr.length = 1019;
    idtr.base_addr = IDT_START;
    asm volatile("lidtl %0"::"m"(idtr));
}

//空中断处理函数 ，什么都不做，直接返回
void __attribute__((regparm(0))) empty_int(){
    asm volatile("iret;");
}
