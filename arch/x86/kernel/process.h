#ifndef ARCH_X86_BOOT_PROCESS_H_
#define ARCH_X86_BOOT_PROCESS_H_

#include "../../../include/type.h"

struct __attribute__((packed)) tss{
	u16 pre_tss,void1;
	u32 esp0;
	u16 ss0,void_ss0;
	u32 esp1;
	u16 ss1,void_ss1;
	u32 esp2;
	u16 ss2,void_ss2;
	u32 cr3;
	u32 eip;
	u32 eflags;
	u32 eax;
	u32 ecx;
	u32 edx;
	u32 ebx;
	u32 esp;
	u32 ebp;
	u32 esi;
	u32 edi;
	u16 es,void_es;
	u16 cs,void_cs;
	u16 ss,void_ss;
	u16 ds,void_ds;
	u16 fs,void_fs;
	u16 gs,void_gs;
	u16 ldt,void_ldt;
	u16 low_io,io;
};

void make_gdt_descriptor();

#endif /* ARCH_X86_BOOT_PROCESS_H_ */
