#ifndef DRIVERS_KEYBOARDS_H_
#define DRIVERS_KEYBOARDS_H_

#include "../include/type.h"
#include "../include/asm_help.h"
#include "console.h"
#include "../arch/x86/kernel/apic/apic.h"

#define CAPS_LOCK 1     //大写键按下
#define L_SHIFT 2       //左Shift键按下
#define R_SHIFT 4       //右Shift键按下
#define L_CTRL 8        //左Ctrl键按下
#define R_CTRL 16       //右Ctrl键按下
#define L_ALT 32        //左Alt键按下
#define R_ALT 64        //右Alt键按下
#define CAPS_STAT 128   //大写开启状态

struct key_list{
    struct key_list *pre,*next;
    u8 key_code;
};

struct __attribute__((packed)) code_ascii_list{
    u8 all_ascii[83];
};

typedef void __attribute__((regparm(0))) (*A_FUNC)();

void set_trap_gate(u8 int_num,A_FUNC func);

void init_interrupt();

void __attribute__((regparm(0))) keyboard_interrupt();

#endif
