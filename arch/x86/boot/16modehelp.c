#include "init.h"

/*
 *虽然这样做会很耗费时间，但是鉴于我们很快就会进入保护模式；
 *这种函数调用不多，效率低点可以考虑。
 */
void fastcall16 bios_put_cha(char cha){
	struct all_regs reg;
	reg.al = cha;
	reg.ah = 0x0e;
	reg.bx = 0x0007;
	intcall(0x13,&reg);
}

void bios_put_str(char *str){
	while(*str++)
		bios_put_cha(*(str - 1));
}
