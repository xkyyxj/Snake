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
	movl 60(%esp),%edx;
	movl %edx,%edi;
	movl %esp,%esi;
	movl $9,%ecx;
	rep movsd;
	
	#恢复esp寄存器
	addl $36,%esp;

	popfl;
	popal;

	movl %ebp,%esp;
	popl %ebp;

	retl;	#此处使用的是近返回，只返回了IP，没有返回CS；但是压栈时压入了IP和CS，导致栈混乱
			#此处修改使用远返回，发现Linux源码也是这样做的，为什么呢？？
