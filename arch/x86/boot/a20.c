/*
 * a20.c
 *
 *  Created on: 2017年1月10日
 *      Author: wangqchf
 */

#include "init.h"

u8 test_a20(){
	u8 ok = 0;
	u16 content = 0xabcd,saved = 0;
	//写入地址是引导扇区加载的地址
	u16 addr = 0x7c00;
	ldfs(0x0000);
	ldgs(0xffff);
	wrfsw(content,addr);
	addr += 0x10;
	saved = rdgsw(addr);
	//asm volatile("movw %1,%%fs:%2;"
	//			"addw %2,$0x10;"
	//			"movw %%gs:%2,%0":"=c"(saved):"a"(content),"m"(addr));
	if(saved ^ content)
		ok = 0;
	else
		ok = 1;
	return ok;
}

void open_a20_fast(){
	asm volatile("in $0x92,%%al; orb $0x02,%%al; out %%al,$0x92":::"ax");
}

void open_a20(){
	u8 is_ok = 0;
	char str[] = "1234";
	bios_put_str(str);
	open_a20_fast();
	if(!test_a20()){

	}
}
