#include "Keyboard.h"

static struct code_ascii_list list = {
    {0,'1','2','3','4','5','6','7','8','9','0','-','=',0,0,
    'q','w','e','r','t','y','u','i','o','p','[',']',0,0,'a','s',
    'd','f','g','h','j','k','l',';','\'','`',0,'\\','z','x','c','v',
    'b','n','m',',','.','/',0,0,0,32,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,'-',0,0,0,'+',0,
    0,0,0,0}
};

static struct code_ascii_list shift_list = {
    {0,'!','@','#','$','%','^','&','*','(',')','_','+',0,0,
    'Q','W','R','R','T','Y','U','I','O','P','{','}',0,0,'A','S',
    'D','F','G','H','J','K','L',':','"','~',0,'|','Z','X','C','V',
    'B','N','M','<','>','?',0,0,0,32,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,'-',0,0,0,'+',0,
    0,0,0,0}
};

static u8 key_state = 0;    //按键状态，对应于上述的八种按键是否被按下等

static u8 code_state;

static struct key_list *head = 0,*tail = 0;

//初始化中断例程 键盘中断号对应为0x21
void init_interrupt(){
    key_state = 0;//key_state属于.bss段，现在先在代码里初始化
    set_trap_gate(0x21,&keyboard_interrupt);
}

//向屏幕上打印字节内容，但是显示顺序与正常二进制数相反了……
void print_byte(u8 content){
    u8 print_con = 0;
    for(int i = 0;i < 8;i++){
        print_con = content % 2;
        content /= 2;
        put_char(print_con + 48);
    }
}

u8 key_code_handler(u8 code){
    u8 return_code = 0;
    //代表这是断开扫描码
    if(code > 0x80){
        code -= 0x80;
        switch(code){
        case 0x2a:
            key_state &= ~L_SHIFT;
            break;
        case 0x36:
            key_state &= ~R_SHIFT;
            break;
        }
        return 0;
    }
    //代表是按下扫描码
    else{
        switch(code){
        case 0x2a://左shift键
            key_state |= L_SHIFT;
            return return_code;
        case 0x36://右shift键
            key_state |= R_SHIFT;
            return return_code;
        case 0x3a://大写键
            if(key_state & CAPS_STAT)
                key_state &= ~CAPS_STAT;
            else
                key_state |= CAPS_STAT;
            return return_code;
        }
        //print_byte(key_state);
        //扫码码从01开始而数组索引从0开始，故而此处减一
        if(key_state & CAPS_STAT){
            if(((code >= 0x10) && (code <= 0x19)) || ((code >= 0x1e) && (code <= 0x26)) || ((code >= 0x2c) && (code <= 0x32))){
                if((key_state & L_SHIFT) || (key_state & R_SHIFT))          
                    return list.all_ascii[code - 1];
                else
                    return shift_list.all_ascii[code - 1];
            }
            else
                if((key_state & L_SHIFT) || (key_state & R_SHIFT))   
                    return shift_list.all_ascii[code - 1];
                else
                    return list.all_ascii[code - 1];
        }
        else{
            if((key_state & L_SHIFT) || (key_state & R_SHIFT))
                return shift_list.all_ascii[code - 1];
            else
                return list.all_ascii[code - 1];
        }
    }
}

//键盘中断处理程序
//对于这种中断处理函数应该用汇编写的（这是由在empty_int函数当中打印字符串得到的呃灵感）
//像是这个函数，在这里我们在函数当中声明了许多的变量，然后栈指针向下减少
//然后调用了iret指令，那么肯定是返回地址错误了啊
/*void __attribute__((regparm(0))) keyboard_interrupt(){
    u8 code = 0;
    code = io_inb(0x60);//读入扫描码
    put_string("what1234");
    io_apic_eoi();
    asm volatile("iret");
}*/
