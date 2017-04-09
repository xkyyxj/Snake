#include "init.h"

void fastcall16 bios_put_cha(char cha){
	struct all_regs reg;
	//init_regs(&reg);
	reg.al = cha;
	reg.ah = 0x0e;
	reg.bx = 0x0007;
	reg.cx = 0x0001;
	intcall(0x10,&reg);
}

void bios_put_str(char *str){
	while(*str++)
		bios_put_cha(*(str - 1));
}

void io_delay(){
	u16 port = 0x80;
	asm volatile("outb %%al,%0"::"dN"(port));
}

void loader_memcpy(u32 start, u32 destination, u32 length){
	//原先是想用movsl的，但是看网上帖子好像没有movsl？而且用movsl会导致不能够正确的移动数据
	//而且在bochs上会导致sp莫名其妙的加1，导致返回地址错误；在virtualbox上也不能正常运行
	asm volatile("rep movsb;"::"c"(length),"D"(destination),"S"(start));
}
