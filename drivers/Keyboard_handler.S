put_content:
    .long 0x77686174;
    .byte 0;

.globl keyboard_interrupt
.type keyboard_interrupt, @function
keyboard_interrupt:
    pushl %eax;
    pushl %edx;

    #pushl $put_content;
    #call put_string;
    #addl $4,%esp;
    #刚刚有个问题就是中断响应只能进行一次，后续无响应
    #但是现在从端口读出键盘扫描码之后就可以多次响应中断了……
    movl $0x60,%edx;
    inb %dx,%al;

    subl $4,%esp;
    movl %eax,(%esp);
    call key_code_handler;

    andb %al,%al;  #若为0，表明不是可打印字符，则不打印
    jz 1f;
    movl %eax,(%esp);
    call put_char;
1:
    addl $4,%esp;

    #另外就是写EOI Register去掉后中断也不能正常响应
    call io_apic_eoi;

    popl %edx;
    popl %eax;
    iret;
