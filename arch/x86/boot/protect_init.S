#include "init.h"

.code16
.section .text

.globl os_main

pro_init_mes:
	.asciz "protect mode initialize..."

location_gdt:
	.word 0x0000
	.long 0x0000

	.globl init;
init:
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
	movw $0x1000,%ax;		#GDT表位于内存0x10800处
	movw %ax,%ds;
	movl $0x0800,%ebx;		#此处应该是用32位寄存器，不然GAS报错
	movl $0x0000,%edi;

	movl $0x00,(%ebx,%edi,4);	#空GDT项
	inc %edi;
	movl $0x00,(%ebx,%edi,4);

	inc %edi;				#代码段描述符
	movl $0x0000ffff,(%ebx,%edi,4);
	inc %edi;
	movl $0x00c09800,(%ebx,%edi,4);

	inc %edi;				#数据段描述符
	movl $0x0000ffff,(%ebx,%edi,4);
	inc %edi;
	movl $0x00cf9200,(%ebx,%edi,4);

	#ϵͳ��ջ�Σ�TODO��

	movw $0x07c0,%ax;
	movw %ax,%ds;

	movw $location_gdt,%bx;	#设置GDT寄存器的属性
	movw $23,(%bx);
	movl $0x10800,2(%bx)

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

	movl %cr0,%ebx;
	orl $0x0001,%ebx;
	movl %ebx,%cr0;

	.byte 0x66,0xea;	#true long jump!!
1:
	.long in_pm32;
	.word 0x00;

.code32
.globl in_pm32;
in_pm32:
	jmp goto_hlt;
	ret;

	.globl goto_hlt;
goto_hlt:

1:
	hlt;
	jmp 1b;
	ret;			#never use!



