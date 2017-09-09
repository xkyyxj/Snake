#必须通过下面的一行指定是在16位模式的代码，不然的话，下面的对于32位寄存器的操作在bochs当中默认变成了16位寄存器的操作，导致相应的BIOS调用错误
.code16

.globl detect_memory_e820
.type detect_memory_e820, @function
#调用BIOS中断探测内存分布状况
detect_memory_e820:
    pushal;

	#boot以及loader最多占用四个扇区，可以利用第五个扇区存储内存信息
	#最开始的四个字节存储内存区块数
    movw $0x07c0,%ax;
    movw %ax,%es;
    movl $0xA04,%edi;
	
	#用esi寄存器存储内存区块数目
	xorl %esi,%esi;

    movl $0xe820,%eax;
    xorl %ebx,%ebx;
    movl $20,%ecx;
    movl $0x534d4150,%edx;

1:    
    int $0x15;
    #下面两条指令的位置互换，导致了即使出错也不结束，造成死循环（add指令影响CF标志位）
    jb 2f;
    addl $1,%esi;
    cmp %ebx,0;
    jz 3f;

    addl $20,%edi;
    jmp 1b;
    
2:
	#返回值0：表示出错
	popal;
	movl $0, %eax;
	jmp 4f;
3:
	#回写内存区块数目至内存当中
	movl $0xA00,%edi;
	movl %esi,(%edi);
	popal;
4:
    retl;


.globl detect_memory_e801
.type detect_memory_e801, @function
#调用BIOS中断探测内存分布状况
detect_memory_e801:
	pushal;
	movw $0xe801, %ax;

	#清空寄存器
	xorw %bx,%bx;
	xorw %cx,%cx;
	xorw %dx,%dx;

	int $0x15;

	jb 6f;

	cmpw $0, %ax;
	jne 5f;					#使用ax以及bx两个寄存器

	movw %cx,%ax;			#使用cx以及dx两个寄存器，写入到ax以及bx
	movw %dx,%bx;
5:
	#将内存规模数据写入到同e820调用相同的位置，然后转入到C代码当中后处理成
	#memory.h当中指定的结构体addr_range_desc的形式
	movw $0x07c0,%ax;
    movw %ax,%es;
    movl $0xA04,%edi;
    movw %ax,(%edi);
    addl $2,%edi;
    movw %bx,(%edi);
    #将内存区块数量写入到最开始的四个字节当中
    movl $0xA00,%edi;
    movl $2,(%edi)

	#返回true表明内存探测成功
    popal;
    movl $1,%eax;
    jmp 7f;
6:
	#探测内存报错，或许是不支持e801的BIOS调用
	popal;
	movl $0,%eax;
7:
	retl;
