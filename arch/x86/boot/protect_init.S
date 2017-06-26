#include "init.h"

.code16
.section .text

.globl os_main

pro_init_mes:
	.asciz "protect mode initialize......"

pro_finish:
	.asciz "protect will finish.."

location_gdt:
	.word 0x0000
	.long 0x0000

	.globl init;
init:
	movw $0x07c0,%ax;		#因为读硬盘的时候修改了ds寄存器，此处修正过来
	movw %ax,%ds;

	movw $0x0200,%dx;		#设置光标位置：3行1列
	xorw %bx,%bx;
	movw $0x0200,%ax;
	int $0x10;

	movw $pro_init_mes,%si;	#修正数据段寄存器，所以此处使用汇编偏移地址

pro_print:					#打印信息
	cld;
	lodsb;
	andb %al,%al;
	jz os_main;
	movb $0x0e,%ah;
	int $0x10;
	jmp pro_print;

	.globl setup_gdt;
setup_gdt:					#设置GDT
	pushl %ebp;
	movl %esp,%ebp;

	pushl %edi;
	movw $0x2000,%ax;		#GDT表位于内存0x20000处
	movw %ax,%ds;
	movl $0x0000,%ebx;		#此处应该是用32位寄存器，不然GAS报错
	movl $0x0000,%edi;

	movl $0x00,(%ebx,%edi,4);	#空GDT项
	inc %edi;
	movl $0x00,(%ebx,%edi,4);

	inc %edi;				#代码段描述符
	movl $0x0000ffff,(%ebx,%edi,4);
	inc %edi;
	movl $0x00cf9800,(%ebx,%edi,4);	#此处原先的段属性是0x00409800，故而kernel开始地址0x10000超出段界限
									#这个bug好烦人（kernel的加载地址由kernel.ld文件指定，并由loader_main.c解析）

	inc %edi;				#数据段描述符(基地址更改为加载地址，否则访问静态数据变量是会有错误的地址（根据汇编地址来的）)
	movl $0x0000ffff,(%ebx,%edi,4);
	inc %edi;
	movl $0x00cf9200,(%ebx,%edi,4);

	inc %edi;				#堆栈段描述符
	movl $0x7c00ffff,(%ebx,%edi,4);
	inc %edi;
	movl $0x00409200,(%ebx,%edi,4);

	movw $0x07c0,%ax;
	movw %ax,%ds;

	movw $location_gdt,%bx;	#设置GDT寄存器的属性
	movw $31,(%bx);
	movl $0x20000,2(%bx)

	lgdt location_gdt;

	popl %edi;
	movl %ebp,%esp;
	popl %ebp;

	ret;

	.globl setup_idt;
setup_idt:				#设置IDT
	pushw $0;
	pushl $0;
	lidt (%esp);
	addl $6,%esp;
	ret;

	.globl goto_protect;
goto_protect:
	pushl %eax;
	pushl %ebx;

	movw $1f,%bx;

	xorl %eax,%eax;
	movw %ds,%ax;
	shll $4,%eax;
	addl %eax,1f;

	movw $0x0008,4(%bx);

	cli;

	movw $pro_finish,%si;

pro_print1:
	cld;
	lodsb;
	andb %al,%al;
	jz set_cr0;
	movb $0x0e,%ah;
	int $0x10;
	jmp pro_print1;

set_cr0:
	movl %cr0,%ebx;
	orl $0x0001,%ebx;
	movl %ebx,%cr0;
go_pro:

	.byte 0x66,0xea;	#true long jump!!
1:
	.long in_pm32;
	.word 0x00;

.code32
.globl in_pm32;
in_pm32:
	movw $0x10,%ax;
	movw %ax,%ds;
	movw $0x18,%ax;
	movw %ax,%ss;

	movw $0x7e00,%eax;
	jmpl *(%eax);		#以前错误的写成了jmpl *%eax，这样会跳转到0x7e00处执行，导致错误
	ret;

	.globl goto_hlt;
goto_hlt:

1:
	hlt;
	jmp 1b;
	ret;			#never use!

.globl read_disk
.type read_disk, @function
read_disk:			#es:di 指向内存地址 ax：读写扇区数
	pushal;

	movw %ax,%bx;	#保存读取扇区数

	movw $0x01f1,%dx;
	movw $0x00,%ax;
	out %al,%dx;
	out %al,%dx;
		
	movw $0x01f2,%dx;#读磁盘的扇区数
	movw %bx,%ax;
	movw $8,%cx;
	shrw %cl,%ax;
	out %al,%dx;
	movw %cx,%ax;	
	out %al,%dx;

	movb $0x00,%al;
	movw $0x01f3,%dx;
	out %al,%dx;

	movb $0x01,%al;
	out %al,%dx;

	movw $0x01f4,%dx;
	movb $0x00,%al;
	out %al,%dx;

	out %al,%dx;

	movw $0x01f5,%dx;
	out %al,%dx;
	out %al,%dx;

	movw $0x01f6,%dx;
	movb $0x40,%al;
	out %al,%dx;

	movw $0x1f7,%dx;
	movb $0x24,%al;
	out %al,%dx;
