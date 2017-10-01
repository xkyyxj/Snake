#include "init.h"
#include "../../../include/elf_info.h"

boot_info* sys_info = (boot_info*)0xA00;

void os_main(){

	setup_gdt();

	//setup_idt();

	open_a20();

	detect_memory();

	//不知道这种做法好不好，boot和loader总共合计占据五个扇区，五个扇区后就是kernel
	//故而有下面的地址（这个是由build.c控制的）
	analyze_kernel(BOOT_LOADER_LENGTH + MAX_SYSTEMINFO_LENGTH);

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

	sys_info->kernel_start = phdr->p_vaddr;
	sys_info->kernel_end = phdr->p_vaddr + phdr->p_memsz;

	//最后执行跳转，跳转到kernel当中
	//asm volatile("ljmp *%0"::"m"(start));
}

//内存探测函数
void detect_memory() {
	/*if(detect_memory_e820())
		return;
	else if(detect_memory_e801()) {
		//将e801方式探测到的内存转换为e820格式，只有两个16位的数字，内存地址保存地址见mem_detect.S文件
		u16 less_16M_num = *((u16*)(BOOT_LOADER_LENGTH + 4));
		u16 more_16M_num = *((u16*)(BOOT_LOADER_LENGTH + 6));

		struct loader_addr_range_desc* mem_range = (struct loader_addr_range_desc*)(BOOT_LOADER_LENGTH + 4);

		//低于16M的内存地址
		mem_range->base_addr_low = 0x100000;//低于16M的内存地址是从1M起始么？
		mem_range->base_addr_high = 0;
		mem_range->length_low = less_16M_num << 10;	//低于16M是以KB为单位的，此处转为以B为单位
		mem_range->length_high = 0;
		mem_range->type = 1;	//对应于e820当中的AddressRangeMemory，表示内存可用

		//高于16M的内存地址
		mem_range++;
		mem_range->base_addr_low = 0x1000000;//高于16M的内存地址应当是从16M起始的吧
		mem_range->base_addr_high = 0;
		mem_range->length_low = less_16M_num << 16;	//高于16M是以64KB为单位的，此处转为以B为单位
		mem_range->length_high = 0;
		mem_range->type = 1;	//对应于e820当中的AddressRangeMemory，表示内存可用

		return;
	}
	else
		//TODO -- 最原始的BIOS探测内存方式实现
		return;*/

	if(detect_mem_e820())
		return;
	else if(detect_mem_e801())
		return;
	else
		//TODO --最原始的内存探测方式
		return;

}

bool detect_mem_e820() {
	u8 block_count = 0;
	struct all_regs reg;
	loader_memset(&reg,sizeof(struct all_regs),0);
	
	reg.edi = BOOT_LOADER_LENGTH + sizeof(boot_info);
	reg.eax = 0xe820;
	reg.ecx = 20;
	reg.edx = 0x534d4150;
	
	do{
		intcall(0x15,&reg);

		//CF进位标识被置位，表明出错
		if(reg.eflags & CF_FLAGS)
			return 0;

		if(reg.edx != 0x534d4150)
			return 0;

		block_count++;
	}
	while(reg.ebx != 0);

	sys_info->mem_block_number = block_count;

	return 1;
}

bool detect_mem_e801() {
	struct all_regs reg;
	loader_memset(&reg,sizeof(struct all_regs),0);

	reg.eax = 0xe801;

	intcall(0x15,&reg);

	if(reg.eflags & CF_FLAGS)
		return 0;
	
	//低于16M的物理内存当中可用内存不可能大于15M
	if(reg.ax > 15 * 1024)
		return 0;

	if(reg.ax == 0 || reg.bx == 0) {
		reg.ax = reg.cx;
		reg.bx = reg.dx;
	}

	//下面的问题需要纠正一下，似乎在实模式下用32位寄存器进行移位操作是不可行，因此下面如果移位的话会产生错误数据
	/*struct loader_addr_range_desc* mem_range = (struct loader_addr_range_desc*)(BOOT_LOADER_LENGTH + sizeof(boot_info));
	mem_range->base_addr_low = 0x100000;//低于16M的内存地址是从1M起始么？
	mem_range->base_addr_high = 0;
	mem_range->length_low = reg.eax << 10;	//低于16M是以KB为单位的，此处转为以B为单位
	mem_range->length_high = 0;
	mem_range->type = 1;	//对应于e820当中的AddressRangeMemory，表示内存可用

	mem_range++;

	mem_range->base_addr_low = 0x1000000;//高于16M的内存地址应当是从16M起始的吧
	mem_range->base_addr_high = 0;
	mem_range->length_low = reg.ebx << 16;	//高于16M是以64KB为单位的，此处转为以B为单位
	mem_range->length_high = 0;
	mem_range->type = 1;	//对应于e820当中的AddressRangeMemory，表示内存可用*/
	
	struct loader_addr_range_desc* mem_range = (struct loader_addr_range_desc*)(BOOT_LOADER_LENGTH + sizeof(boot_info));
	mem_range->base_addr_low = 0x400;
	mem_range->base_addr_high = 0;
	mem_range->length_low = reg.eax + (reg.ebx << 6);
	mem_range->length_high = 0;
	mem_range->type = 1;	//对应于e820当中的AddressRangeMemory，表示内存可用

	sys_info->mem_block_number = 1;

	return 1;
}
