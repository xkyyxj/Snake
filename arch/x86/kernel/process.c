#include "process.h"

//GDTR寄存器当中的内容
static struct gdt current_gdt = {4 * 8 - 1,0x10800};

void make_init(){
	make_kernel_tss();
}

void set_tss_des(struct tss *input){

}

void make_kernel_tss(){
	//创建kernel的TSS（内存地址紧随IDT）
	struct tss *new_tss = (struct tss*)1020;

	//新建特权级堆栈
	u32 low_content = 0x0000ffff;
	u32 high_content = 0x00409202;
	u16 stack_index = install_seg_descriptor(1,low_content,high_content);

	new_tss->ss0 = stack_index;
	new_tss->esp0 = 0xfff;
	new_tss->pre_tss = 0;

	//新建进程0的附加数据段描述符（用于某些指令，如stosl）
	high_content = 0x00c09200;
	u16 es_index = install_seg_descriptor(1,low_content,high_content);
	//加载描述到ES寄存器
	asm volatile("movw %0,%%es"::"r"(es_index));
	//加载kernel的TSS到TSS段寄存器
	/*struct tss_entry entry;
	entry.length = 103;
	entry.base_addr = 1020;
	asm volatile("ltrl %0"::"m"(entry));*/
	low_content = 0;
	high_content = 0;
	low_content = (1020 << 16) & 103;
	high_content = 0x00008900;
	u16 tr_index = install_seg_descriptor(1,low_content,high_content);
	//加载TR寄存器
	asm volatile("ltrw %0"::"r"(tr_index));
}

void make_stack_descriptor(u32 base_addr, u32 length, u8 s_dpl_type){
	u32 low_content = base_addr & 0xffff & ((length & 0xffff) << 16);
	u32 high_content = 0;
	high_content = (base_addr & 0xff0000) >> 16;
	high_content = high_content & (s_dpl_type & 0x0f << 4);
	high_content = high_content & (s_dpl_type);
}

void make_tss(){
	struct tss new_tss;
	
}

u16 install_seg_descriptor(bool in_gdt,u32 low_content,u32 high_content){
	u16 des_index = 0;
	if(in_gdt) {
		//下面的代码相当丑陋：首先就是内核被加载到0x7c00地址处，访问静态数据变量的指针是汇编地址，故而需要DS寄存器是0x7c00
		//其次就是因为DS寄存器的内容是0x7c00，故而在写GDT内容的时候，需要将GDT基地址减去DS寄存器地址……
		u32 *write_addr = (u32*)(current_gdt.length + 1 + current_gdt.base_addr - 0x7c00);
		*write_addr = low_content;
		*(write_addr + 1) = high_content;
		current_gdt.length += 8;
		asm volatile("lgdtl %0"::"m"(current_gdt));
		//默认是特权级0（位于GDT当中）
		des_index = (((current_gdt.length + 1) / 8 - 1) << 3);
		return des_index;
	}
	return 0;
}
