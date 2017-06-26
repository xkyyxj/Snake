.globl detect_memory
.type detect_memory, @function
#调用BIOS中断探测内存分布状况
detect_memory:
    pushal;

	#boot以及loader最多占用四个扇区，可以利用第五个扇区存储内存信息
	#最开始的四个字节存储内存区块数
    movw $0x07c0,%ax;
    movw %ax,%es;
    movl $2052,%edi;
	
	#用esi寄存器存储内存区块数目
	xorl %esi,%esi;

    movl $0xe820,%eax;
    xorl %ebx,%ebx;
    movl $20,%ecx;
    movl $0x534d4150,%edx;

1:    
    int $0x15;
	addl $1,%esi;
    jb 2f;
    cmp %ebx,0;
    jz 2f;

    addl $20,%edi;
    jmp 1b;
    
2:
	#回写内存区块数目至内存当中
	movl $2048,%edi;
	movl %esi,(%edi);
	popal;
    retl;