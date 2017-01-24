/*
 * init.h
 *
 *  Created on: 2017��1��7��
 *      Author: wangqchf
 */

#include "type.h"

#ifndef ARCH_X86_BOOT_INIT_H_
#define ARCH_X86_BOOT_INIT_H_

#define fastcall16 __attribute__((regparm(3)))

struct all_regs{
	union{
		struct{
			u32 edi;
			u32 esi;
			u32 ebp;
			u32 esp;
			u32 ebx;
			u32 edx;
			u32 ecx;
			u32 eax;
			u32 eflags;
		};

		struct{
			u16 di,hdi;
			u16 si,hsi;
			u16 bp,hbp;
			u16 sp,hsp;
			u16 bx,hbx;
			u16 dx,hdx;
			u16 cx,hcx;
			u16 ax,hax;
			u16 flags,hflags;
		};

		struct{
			u8 di1,di2,di3,di4;
			u8 si1,si2,si3,si4;
			u8 bp1,bp2,bp3,bp4;
			u8 sp1,sp2,sp3,sp4;
			u8 bl,bh,hbl,hbh;
			u8 dl,dh,hdl,hdh;
			u8 cl,ch,hcl,hch;
			u8 al,ah,hal,hah;
		};
	};
};

//void detect_memory(); TODO detect memory!!

void goto_protect();

void setup_gdt();

void setup_idt();

void open_a20();

void goto_hlt();		//for test

void bios_put_str(char *str);

void fastcall16 intcall(u8 call_nu,struct all_regs *regs);

static inline void wrfsw(u16 content,u16 addr){
	asm volatile("movl %1,%%fs:%0;"::"m"(addr),"a"(content));
}

static inline u16 rdgsw(u16 addr){
	u16 ret_value = 0;
	asm volatile("movl %%gs:%1,%0":"=r"(ret_value):"m"(addr));
	return ret_value;
}

static inline u8 fastcall16 inb(u16 port){
	u8 return_value;
	asm volatile("inb %1,%0;":"=a"(return_value):"d"(port));
	return return_value;
}

static inline void fastcall16 outb(u8 out,u16 port){
	asm volatile("outb %al,%dx;");
}

static inline void fastcall16 ldfs(u16 fs){
	asm volatile("movw %ax,%fs;");
}

static inline void fastcall16 ldgs(u16 gs){
	asm volatile("movw %ax,%gs;");
}

#endif /* ARCH_X86_BOOT_INIT_H_ */
