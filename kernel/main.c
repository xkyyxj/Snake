#include "../drivers/console.h"
#include "../drivers/Keyboard.h"
#include "../arch/x86/kernel/process.h"
#include "../arch/x86/kernel/traps.h"
#include "../arch/x86/kernel/apic/apic.h"
#include "../arch/x86/kernel/mm/memory.h"

/**
 * kernel entry
 */
void kernel_main(){
	
	//初始化内存（见memory.c）
	init_mem_info();
	//初始化0号进程(见process.c)
    make_init();
    console_init();
    trap_init();
    init_io_apic();
    init_interrupt();
	//打印内存信息，检测内存检测程序是否能够正常工作
	print_memory();

    //测试console.c打印字符串功能
    put_string("Welcome to Snake!");
    next_line();

    //put_string("what?");

    //打开中断
    asm volatile("sti;");

    //停机，除非中断到来（目前只有键盘中断会被响应？）
goto_hlt:
    asm volatile("hlt;");
    goto goto_hlt;
}
