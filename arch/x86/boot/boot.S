/*
 * boot.asm
 *
 *  Created on: 2016年12月24日
 *      Author: wangqchf
 */

.code16
.section .btext,"ax"
.globl _start
_start:
	movw $0x07c0,%ax;	#设置堆栈段地址
	movw %ax,%ss;
	movw $0x07c0,%ax;	#设置数据段地址
	movw %ax,%ds;
	movw $0xb800,%ax;	#设置附加段地址指向显示区
	movw %ax,%es;

	movw $0x0000,%sp;	#栈段长度 = 640K - 实模式代码长度

	movw $loading,%si;
	movw $0,%di;		#设置di指向缓冲区开始

print_str_:
	cld;
	lodsb;
	andb %al,%al;
	jz next;
	movb $0x0e,%ah;
	movw 0x0007,%bx;
	int $0x10;
	jmp print_str_;

next:
	movw $0,%ax;
	call lba48;

	#ljmp $0x1000,$0x0000;
	jmp init;

	movw $0x0700,%ax;
	movw %ax,%ds;
	movw $0,%si;

print_disk:
	cld;
	lodsb;
	andb %al,%al;
	jz stop;
	movb $0x0e,%ah;
	int $0x10;
	jmp print_disk;

stop:					#停机（非正常现象）
	hlt;
	jmp stop;

loading:
	.asciz "loading..."

lba24:
	movw $0x01f2,%dx;
	movw $0x01,%ax;
	out %al,%dx;

	movw $0x01f3,%dx;
	movw $0x0001,%ax;
	out %al,%dx;

	movw $0x01f4,%dx;
	movb $0x00,%al;
	out %al,%dx;

	movw $0x01f5,%dx;
	out %al,%dx;

	movw $0x01f6,%dx;
	movb $0xe0,%al;
	out %al,%dx;

	movw $0x1f7,%dx;
	movb $0x20,%al;
	out %al,%dx;

wait_ready24:
	in %dx,%al;
	andb $0x88,%al;
	cmpb $0x08,%al;
	jnz wait_ready24;

	movw $256,%cx;
	movw $0x07df,%bx;
	movw %bx,%ds;
	movw $0x0f,%bx;		#加载下一个扇区到该扇区之后
						#如此一来就可以用jmp指令了吧
disk_ready24:
	movw $0x01f0,%dx;
	in %dx,%ax;
	movw %ax,(%bx);
	inc %bx;
	inc %bx;
	loop disk_ready24;
	ret;

lba48:					#LBA48模式的读硬盘
						#ax
	movw $0x01f1,%dx;
	movw $0x00,%ax;
	out %al,%dx;
	out %al,%dx;
		
	movw $0x01f2,%dx;
	movw $0x0000,%ax;
	out %al,%dx;
	movw $0x0005,%ax;
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
		
wait_ready48:
	in %dx,%al;
	andb $0x88,%al;
	cmpb $0x08,%al;
	jnz wait_ready48;
	
	movw $0x07e0,%bx;
	movw %bx,%ds;
	movw $0x00,%bx;
	movw $5,%si;
disk_ready48:
	movw $256,%cx;

read_disk:
	movw $0x01f0,%dx;
	in %dx,%ax;
	movw %ax,(%bx);		#此处原为movb %al,(%bx);，导致了一个错误
						#就是程序只显示了一个“s”字母，然后就结束了，因为其后的内存字节是个0
						#But，why???发现好像内存很多的区域都被0填充了，是整个内存都被0填充了？？
	inc %bx;
	inc %bx;
	loop read_disk;
	
	decw %si;
	andw $0xff,%si;	# read five blocks
	jnz disk_ready48;

	ret;

.org 510
.word 0xAA55
