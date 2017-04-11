#include "apic.h"

static volatile u32 *_addr_select = (u32*)0xfec00000;

static volatile u32 *_addr_win = (u32*)0xfec00010;

void write_register(u32 addr, u32 content){
    //io_outl(0xfeco0000,addr);
    //io_outl(0xfeco0010,content);
    *_addr_select = addr;
    *_addr_win = content;
}

u32 read_register(u32 addr){
    u32 *addr_posi = (u32*)addr;
    u32 return_value = *addr_posi;
    return return_value;
}

void create_table_entry(){

}

void init_io_apic(){
    u32 low_keyboard_ent = 0x2021;
    write_register(0x12,low_keyboard_ent);
    //初始化IO-APIC-ID Register
    write_register(0x00,0);
    //获取Local-apic的ID
    u32 local_apic_id = read_register(0xfee00020);
    local_apic_id = (local_apic_id >> 24) & 0xff;
    //键盘中断在io-apic当中的中断向量表项
    //u32 low_keyboard_ent = 0x2021;
    u32 high_keyboard_ent = 0x0 | (local_apic_id << 24);
    //写入键盘中断处理程序的io-apic向量
    //write_register(0x12,low_keyboard_ent);
    write_register(0x13,high_keyboard_ent);
    //u32 low_keyboard_ent = 0x2021;
    //write_register(0x12,low_keyboard_ent);
}