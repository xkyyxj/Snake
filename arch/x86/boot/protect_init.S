/*
 * protect_init.s
 *
 *  Created on: 2017Äê1ÔÂ5ÈÕ
 *      Author: wangqchf
 */


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
	movw $0x0200,%dx;		#ÉèÖÃ¹â±êÎ»ÖÃ£º3ĞĞ1ÁĞ
	xorw %bx,%bx;
	movw $0x0200,%ax;
	int $0x10;

	movw $0x0000,%si;		#²»Ó¦¸ÃÊ¹ÓÃ»ã±àµØÖ·£¬ÒòÎªds²»Îª0x0000
	movw $0x07e0,%ax;		#ÉèÖÃÊı¾İ¶ÎµØÖ·Îª¼ÓÔØµÄÄÚ´æÎ»ÖÃ
	movw %ax,%ds;

pro_print:					#´òÓ¡Ïà¹ØĞÅÏ¢
	cld;
	lodsb;
	andb %al,%al;
	jz os_main;
	movb $0x0e,%ah;
	int $0x10;
	jmp pro_print;
	
	.globl setup_gdt;
setup_gdt:					#Î´±£»¤Êı¾İ¶Î¼Ä´æÆ÷£¬¿ÉÄÜÓĞÎÊÌâ
	pushl %ebp;
	movl %esp,%ebp;

	pushl %edi;
	movw $0x1000,%ax;		#½«GDT±í¸ñÉèÖÃÔÚ0x10800¾ø¶ÔµØÖ·´¦
	movw %ax,%ds;
	movl $0x0800,%ebx;		#´Ë´¦±ØĞëÊ¹ÓÃ32Î»¼Ä´æÆ÷£¬²»È»±äÖ·Ñ°Ö·GAS±¨´í
	movl $0x0000,%edi;

	movl $0x00,(%ebx,%edi,4);	#µÚÒ»¸ö¿Õ°×GDTÏî
	inc %edi;
	movl $0x00,(%ebx,%edi,4);

	inc %edi;				#´úÂë¶ÎGDTÏî
	movl $0x0000ffff,(%ebx,%edi,4);
	inc %edi;
	movl $0x00c09800,(%ebx,%edi,4);

	inc %edi;				#È«¾ÖÊı¾İ¶ÎGDTÏî
	movl $0x0000ffff,(%ebx,%edi,4);
	inc %edi;
	movl $0x00cf9200,(%ebx,%edi,4);

	#ÏµÍ³¶ÑÕ»¶Î£¨TODO£©

	movw $0x07c0,%ax;
	movw %ax,%ds;

	movw $location_gdt,%bx;	#ÉèÖÃGDTÏàÓ¦ÊôĞÔ
	movw $23,(%bx);
	movl $0x10800,2(%bx)		#movl $0x10000,2(%bx) ¿¿¿¿¿

	lgdt location_gdt;		#¼ÓÔØGDTR

	popl %edi;
	movl %ebp,%esp;
	popl %ebp;

	ret;
	
	.globl setup_idt;
setup_idt:				#ÉèÖÃ¿ÕIDT±í
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



