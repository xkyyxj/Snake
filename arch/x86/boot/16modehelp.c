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
	asm volatile("rep movsl;"::"c"(length),"D"(destination),"S"(start));
}
