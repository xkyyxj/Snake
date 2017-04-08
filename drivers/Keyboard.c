#include "Keyboard.h"

static struct code_ascii_list list = {
    {0,'1','2','3','4','5','6','7','8','9','0','-','=',0,
    'q','w','e','r','t','y','u','i','o','p','[',']',0,'a','s',
    'd','f','g','h','j','k','l',';','\'','`',0,'\\','z','x','c','v',
    'b','n','m',',','.','/',0,0,0,32,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,'-',0,0,0,'+',0,
    0,0,0,0}
};

static u8 key_state = 0;    //按键状态，对应于上述的八种按键是否被按下等

static u8 code_state;

static struct key_list *head = 0,*tail = 0;

//初始化中断例程 键盘中断号对应为0x21
void init_interrupt(){
    set_trap_gate(0x21,&keyboard_interrupt);
}

//键盘中断处理程序
void keyboard_interrupt(){
    u8 code = 0;
    code = io_inb(0x60);//读入扫描码
    put_string("what???");
}
