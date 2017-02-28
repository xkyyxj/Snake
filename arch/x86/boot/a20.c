#include "init.h"

u8 test_a20(){
	bios_put_str("test_a20_start");
	u8 ok = 0;
	u16 content = 0xabcd,saved = 0;
	//写入地址为引导扇区所在内存的开始
	u16 addr = 0x7c00;
	bios_put_str("in here???");
	ldfs(0x0000);
	ldgs(0xffff);
	bios_put_str("1");
	wrfsw(content,addr);
	bios_put_str("2");
	addr += 0x10;
	saved = rdgsw(addr);
	bios_put_str("running here??");
	//asm volatile("movw %1,%%fs:%2;"
	//			"addw %2,$0x10;"
	//			"movw %%gs:%2,%0":"=c"(saved):"a"(content),"m"(addr));
	if(saved ^ content)
		ok = 0;
	else
		ok = 1;
	bios_put_str("before return");
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
	bios_put_str("open now!!");
	int i = test_a20();
	test_a20();
	test_a20();
	if(!i){
		//toPosition(6,0);
		bios_put_str("haoba");
		bios_put_str("haoba");
	}
	else {
		bios_put_str("haoba2");
		bios_put_str("haoba2");
	}
	bios_put_str("finish open_a20");
}
