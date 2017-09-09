#ifndef X86_KERNEL_H_
#define X86_KERNEL_H_

//boot传递给内核的参数信息（通过BIOS获取到的以及其他的一些信息）
typedef struct {
	u32 kernel_start,kernel_end;	//内存当中的绝对物理地址
	u32 mem_block_number;			//初始化时的内存区块数
	//剩下的部分存储内存区块数据
} boot_info;

#endif
