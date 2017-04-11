#include "process.h"

//GDTR寄存器当中的内容
static struct gdt current_gdt = {4 * 8 - 1,0x20000};

void make_init(){
	make_kernel_tss();
}

void set_tss_des(struct tss *input){

}

void make_kernel_tss(){
	//创建kernel的TSS（内存地址紧随IDT）
	//此前是在1020处，紧随IDT，但是bochs发现这里有数据，改为0x7c00处
	struct tss *new_tss = (struct tss*)0x7c00;

	//新建特权级堆栈(开始地址：0x20000)
	u32 low_content = 0x0000ffff;
	u32 high_content = 0x00409202;
	u16 stack_index = install_seg_descriptor(1,low_content,high_content);

	//新建进程0的附加数据段描述符（用于某些指令，如stosl）
	high_content = 0x00c09200;
	u16 es_index = install_seg_descriptor(1,low_content,high_content);
	//加载描述到ES寄存器
	asm volatile("movw %0,%%es"::"r"(es_index));

	//清空tss存储区域
	mem_clear(0x7c00,sizeof(struct tss));

	new_tss->ss0 = stack_index;
	new_tss->esp0 = 0xffff;
	new_tss->pre_tss = 0;

	//加载kernel的TSS到TSS段寄存器
	/*struct tss_entry entry;
	entry.length = 103;
	entry.base_addr = 1020;
	asm volatile("ltrl %0"::"m"(entry));*/
	low_content = 0;
	high_content = 0;
	low_content = (0x7c00 << 16) & 103;
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
		u32 *write_addr = (u32*)(current_gdt.length + 1 + current_gdt.base_addr);
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
