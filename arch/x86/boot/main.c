#include "init.h"

void os_main(){
	setup_gdt();

	//setup_idt();

	open_a20();

	goto_protect();

	goto_hlt();
}

