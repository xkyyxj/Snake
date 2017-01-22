/*
 * main.c
 *
 *  Created on: 2017Äê1ÔÂ7ÈÕ
 *      Author: wangqchf
 */

#include "init.h"

void os_main(){
	setup_gdt();

	//setup_idt();

	open_a20();

	goto_protect();

	goto_hlt();
}

