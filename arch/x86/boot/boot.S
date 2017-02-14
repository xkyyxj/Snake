.code16
.section .btext,"ax"
.globl _start
_start:
	movw $0x07c0,%ax;	#栈段寄存器
	movw %ax,%ss;
	movw $0x07c0,%ax;	#数据段寄存器
	movw %ax,%ds;
	movw $0x07c0,%ax;	#修改附加段同数据段等相同，解决movs等指令问题（附加段指向显示区）
	movw %ax,%es;

	movw $0xffff,%sp;	#栈长度 = 640K - 实模式代码长度 - 1

	movw $loading,%si;
	movw $0,%di;

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

stop:
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
	movw $0x0f,%bx;		#加载剩下的扇区到该扇区内存地址之后
						#如此可用相对跳转

	cli;

disk_ready24:
	movw $0x01f0,%dx;
	in %dx,%ax;
	movw %ax,(%bx);
	inc %bx;
	inc %bx;
	loop disk_ready24;
	ret;

lba48:					#LBA48模式磁盘读操作
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
	
	movw $0x07e0,%bx;	#此处修改了ds数据段寄存器，所以导致了许多莫名其妙的错误
	movw %bx,%ds;
	movw $0x00,%bx;
	movw $5,%si;
disk_ready48:
	movw $256,%cx;

read_disk:
	movw $0x01f0,%dx;
	in %dx,%ax;
	movw %ax,(%bx);
	inc %bx;
	inc %bx;
	loop read_disk;
	
	decw %si;
	andw $0xff,%si;	# read five blocks
	jnz disk_ready48;

	ret;

.org 510
.word 0xAA55
