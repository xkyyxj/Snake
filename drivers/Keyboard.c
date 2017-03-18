#include "../include/type.h"
//初始化中断例程 键盘中断号对应为0x21
void init_interrupt(){

}

//键盘中断处理程序
void keyboard_interrupt(){
    u8 code = 0;
    asm volatile("");
}
