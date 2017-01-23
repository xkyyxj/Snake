#include "init.h"

.code16
.globl intcall;
.type intcall, @function
intcall:
	pushl %ebp;
	movl %esp,%ebp;

	movb %al,1f;

	pushal;
	pushfl;

	#copy all register from caller
	subl $36,%esp;
	movl %esp,%edi;
	movl %edx,%esi;
	movl $9,%ecx;
	rep movsd;

	#��ֵ�Ĵ���
	popal;
	popfl;

	.byte 0xcd;	#real interrupt call
1:	.byte 0x00;	#call_number

	popfl;
	popal;
	ret;