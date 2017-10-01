#include "../../../include/type.h"

#ifndef ARCH_X86_BOOT_INIT_H_
#define ARCH_X86_BOOT_INIT_H_

#define fastcall16 __attribute__((regparm(3)))

//定义Boot和Loader的最大长度 (512 * 5，5个扇区的大小)
#define BOOT_LOADER_LENGTH 0xA00

//定义Loader传递给Kernel的信息的最大长度(两个扇区的大小)
#define MAX_SYSTEMINFO_LENGTH 1024

//CF标志位置位的FLAGS
#define CF_FLAGS 1

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

struct loader_addr_range_desc{
    u32 base_addr_low;
    u32 base_addr_high;
    u32 length_low;
    u32 length_high;
	u32 type;
};

void detect_memory();

//C形式的内存探测函数
bool detect_mem_e820();

//C形式的内存探测函数
bool detect_mem_e801();

void goto_protect();

void setup_gdt();

void setup_idt();

void open_a20();

void goto_hlt();		//for test

void bios_put_str(char *str);

void fastcall16 init_regs(struct all_regs* regs);

void fastcall16 intcall(u8 call_nu,struct all_regs *regs);

void loader_memcpy(u32 start, u32 destination, u32 length);

void loader_memset(void* pointer,u8 length,u8 value);

void analyze_kernel(u32 kernel_addr);

//目前仅提供第一页内的光标定位
static inline void toPosition(u8 row,u8 column){
	struct all_regs reg;
	reg.dh = row;
	reg.dl = column;
	reg.ax = 0x0200;
	reg.bx = 0x0000;//置页号为0
	intcall(0x10,&reg);
};

//发觉在Linux代码当中以及网上帖子看到的代码当中，很多写端口的操作后都会调用io_delay
//此处参照了Linux源码当中的io_delay
//原先此处是这样声明的：static inline void io_delay()
//上述的声明方式会导致这个函数没有函数体，所以在汇编语言里面call的时候会有问题（没有函数地址）
//此外就是上一个版本当中会在头文件当中定义函数体，导致多处定义的问题
void io_delay();

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

//boot传递给内核的参数信息（通过BIOS获取到的以及其他的一些信息）
typedef struct {
	u32 kernel_start,kernel_end;	//内存当中的绝对物理地址
	u32 mem_block_number;			//初始化时的内存区块数
	//剩下的部分存储内存区块数据
} boot_info;

extern boot_info* sys_info;

#endif /* ARCH_X86_BOOT_INIT_H_ */
