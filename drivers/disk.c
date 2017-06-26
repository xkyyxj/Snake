#include "disk.h"

//记录本次的硬盘操作属性
static u8 disk_choice = 0;

void init_disk_int(){
	set_trap_gate(0x2e,(INT_FUNC)&disk_interrupt);
}

void disk_int_handler(){
	
}

void disk_write(u8 *content,s32 length){
	//记录本次的硬盘操作属性
	disk_choice = DISK_WRITE;
	
	//每次只写一个扇区的数据
	//为了能够处理4GB长度的数据，所以此处使用了u32数据类型，unsigned类型小心数据越界问题
	while(length > 0){
		if(length > PER_SECTION_BYTE){
			write_one_disk(content);
			content += (PER_SECTION_BYTE / sizeof(u8));
			length -= PER_SECTION_BYTE;
		}
		else{
			write_one_disk(content);
			content += (PER_SECTION_BYTE / sizeof(u8));
			length = 0;
		}
	}
	
}

void write_one_disk(u8 *content){
	
}