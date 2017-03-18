#include "init.h"

#Now we are in protect mode!!!
#首先我们应该先处理分页问题
#对于已经存在的代码，参照Linux设置为线性地址与物理地址相同:-)
#在书写这个问题的时候，我还没有处理好怎么跳转到这里的问题:-(，过会再考虑
.code32
.globl start_32
start_32:
    call init_8259A;


#初始化8258A中断芯片
init_8259A:
    movw $0x20,%dx; #主片ICW1
    movb $0x11,%al;
    outb %al,%dx;
    call io_delay;

    movw $0xa0,%dx; #从片ICW1
    outb %al,%dx;
    call io_delay;

    movw $0x21,%dx; #主片ICW2
    movb $0x20,%al; #起始中断号
    outb %al,%dx;
    call io_delay;

    movw $0xa1,%dx; #从片ICW2
    movb $0x28,%al; #起始中断号
    outb %al,%dx;
    call io_delay;

    movw $0x21,%dx; #主片ICW3
    movb $0x04,%al;
    outb %al,%dx;
    call io_delay;

    movw $0xa1,%dx; #从片ICW3
    movb $0x02,%al;
    outb %al,%dx;
    call io_delay;

    movw $0x21,%dx; #主片ICW4
    movb $0x01,%al;
    outb %al,%dx;
    call io_delay;

    movw $0xa1,%dx; #从片ICW4
    movb $0x01,%al;
    outb %al,%dx;
    call io_delay;

    movw $0x21,%dx; #屏蔽主芯片所有中断
    movb $0xff,%al;
    outb %al,%dx;
    call io_delay;

    movw $0xa1,%dx; #屏蔽从芯片所有中断
    outb %al,%dx;

    ret;
