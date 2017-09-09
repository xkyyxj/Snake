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

	#弹出栈中寄存器内容到寄存器中
	popal;
	popfl;

	.byte 0xcd;	#real interrupt call
1:	.byte 0x00;	#call_number

	#将寄存器重新压入栈中，并且构建C环境当中的变量
	pushfl;
	pushal;

	#重新获取EDX，用于构建C环境结构体变量
	addl $44,%esp;
	movl (%esp),%edx;
	movl %edx,%edi;
	movl %esp,%esi;
	movl $9,%ecx;
	rep movsd;

	#恢复esp指针
	subl $8,%esp;

	popfl;
	popal;

	movl %ebp,%esp;
	popl %ebp;

	retl;	#此处使用的是近返回，只返回了IP，没有返回CS；但是压栈时压入了IP和CS，导致栈混乱
			#此处修改使用远返回，发现Linux源码也是这样做的，为什么呢？？


.globl init_regs;
.type init_regs, @function
init_regs:		#初始化寄存器结构体为0，传入寄存器结构体地址
	pushl %ecx;
	pushl %eax;

	xor %eax,%eax;
	movw $9,%cx;
	rep stosl;

	popl %eax;
	popl %ecx;
	retl;
