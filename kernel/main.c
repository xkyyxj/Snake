#include "../drivers/console.h"
#include "../drivers/Keyboard.h"
#include "../arch/x86/kernel/process.h"
#include "../arch/x86/kernel/traps.h"
#include "../arch/x86/kernel/apic/apic.h"

/**
 * kernel entry
 */
void kernel_main(){
    make_init();
    console_init();
    trap_init();
    init_io_apic();
    init_interrupt();

    //测试console.c打印字符串功能
    put_string("Hello world!!");

    put_string("what?");

    //打开中断
    asm volatile("sti;");

    //停机，除非中断到来（目前只有键盘中断会被响应？）
goto_hlt:
    asm volatile("hlt;");
    goto goto_hlt;
}
