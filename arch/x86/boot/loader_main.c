#include "init.h"
#include "../../../include/elf_info.h"

void os_main(){
	setup_gdt();

	//setup_idt();

	open_a20();

	//不知道这种做法好不好，boot和loader总共合计占据五个扇区，五个扇区后就是kernel
	//故而有下面的地址（这个是由build.c控制的）
	analyze_kernel(512 * 5);

	goto_protect();

	goto_hlt();
}

//现在需要想一个问题：这个内核入口信息放到内存的哪个位置呢？
struct __attribute__((packed)) kernel_start{
	u32 entry;				//入口地址
	u16 code_selector;		//代码段选择子
};

void analyze_kernel(u32 kernel_addr){
	//elf header总是位于elf文件的开始
	struct Elf32_Ehdr *ehdr;
	ehdr = (struct Elf32_Ehdr*)kernel_addr;

	//获取程序入口地址
	struct kernel_start *start = (struct kernel_start*)0x200;
	start->entry = ehdr->e_entry;
	start->code_selector = 0x08;

	//获取Program header table的信息
	u32 e_poff = ehdr->e_poff;
	u16 e_phsize = ehdr->e_phentsize;
	u16 e_phnum = ehdr->e_phnum;

	//获取Program header table entry并处理
	struct Elf32_Phdr *phdr = (struct Elf32_Phdr*)(kernel_addr + e_poff);
	u32 p_offset, p_vaddr ,p_memsz;
	for(int i = 0;i < e_phnum;i++){
		phdr += i;
		p_offset = phdr->p_offset;
		p_vaddr = phdr->p_vaddr;
		p_memsz = phdr->p_memsz;
		if(p_memsz > 0){
			//附注：因为我们访问kernel_addr使用的是DS寄存器，故而此处使用kernel_addr + p_offset无需担心地址错误
			loader_memcpy(kernel_addr + p_offset,p_vaddr - 0x7c00,p_memsz);
		}
	}

	//最后执行跳转，跳转到kernel当中
	//asm volatile("ljmp *%0"::"m"(start));
}
