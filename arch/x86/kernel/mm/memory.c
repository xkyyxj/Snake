#include "memory.h"
#include "../kernel.h"

static struct memory_info_entry mem_info[MEM_ENTRY_NUM];

static struct memory_info_entry *start_entry = 0;

/*void init_mem_info(){
	u32 mem_lock_num = *(0x7c00 + 2048);
	struct memory_info_entry *temp_entry;
	struct addr_range_desc *mem_info_start = 0x7c00 + 2052;
	for(int i = 0;i < MEM_ENTRY_NUM;i++){
		//TODO-暂未考虑64位内存状况
		temp_entry = mem_info_start->base_addr_low;
		
		u32 mem_length_low = mem_info_start->length_low;
		u32 t_mem_type = mem_info_start->type;
		if(t_mem_type == ADDRESS_RANGE_FREE){
			mem_info[i].base_addr_low = mem_info_start->base_addr_low;
			mem_info[i].length_low = mem_info_start->length_low;
			mem_info[i].type = IN_FREE;
			
			//处理上一个结构体
			if(i > 0){
				mem_info[i].previouse = &mem_info[i - 1];
			}
			else{
				start_entry = temp_entry;
			}
		}
		mem_info_start++;
	}
}*/

void init_mem_info(){
	boot_info* sys_info = (boot_info*)(0x7c00 + BOOT_LOADER_LENGTH);
	//因为ELF格式文件编译出来是采用了平坦模型，故而我们在这儿可以把elf文件占用内存后面的内存区域当做内存数据区域
	mem_info = (struct memory_info_entry*)sys_info->kernel_end;
	u32 mem_lock_num = sys_info->mem_block_number;
	struct memory_info_entry *temp_entry,*previouse_entry = 0;
	struct addr_range_desc *mem_info_start = (struct addr_range_desc*)(0x7c00 + BOOT_LOADER_LENGTH + sizeof(boot_info));
	//TODO --是不是要考虑下内核这个特权客户占用的内存？

	for(int i = 0;i < mem_lock_num;i++){
		//TODO-暂未考虑64位内存状况
		temp_entry = (struct memory_info_entry*)mem_info_start->base_addr_low;
		
		u32 mem_length_low = mem_info_start->length_low;
		u32 t_mem_type = mem_info_start->type;
		if(t_mem_type == ADDRESS_RANGE_FREE){
			//TODO - 下面一行代码是否会有算数溢出情形
			temp_entry->base_addr_low = mem_info_start->base_addr_low + sizeof(struct memory_info_entry);
			temp_entry->length_low = mem_info_start->length_low - sizeof(struct memory_info_entry);
			temp_entry->mem_type = IN_FREE;
			
			//处理上一个结构体
			if(i > 0){
				temp_entry->previouse = previouse_entry;
			}
			else{
				start_entry = temp_entry;
				start_entry->previouse = 0;
			}
			previouse_entry = temp_entry;
			if(i == (mem_lock_num - 1)){
				temp_entry->next = 0;
			}
		}
		mem_info_start++;
	}
}

void print_u32(u32 print_content){
	u8 temp_str[9] = {0};
	u8 temp_content = 0;
	for(int i = 0;i < 8;i++){
		temp_content = (print_content >> (i * 4)) & 0x0f;
		if(temp_content < 0x0a){
			temp_content += 48;
		}
		else{
			temp_content += 65;
		}
		temp_str[8 - i] = temp_content;
	}
	put_string(temp_str);
}

void print_memory(){
	u32 temp_print = 0;
	/*for(int i = 0;i < MEM_ENTRY_NUM;i++){
		//打印内存起始地址
		temp_print = mem_info[i].base_addr_low;
		put_string("base:");
		print_u32(temp_print);
		//打印内存长度
		temp_print = mem_info[i].length_low;
		put_string("length:");
		print_u32(temp_print);
		next_line();
	}*/
	struct memory_info_entry* temp = start_entry;

	while(temp) {
		temp_print = temp->base_addr_low;
		put_string("base:");
		print_u32(temp_print);
		temp_print = temp->length_low;
		put_string("length:");
		print_u32(temp_print);
		temp = temp->next;
	}
}

u32 pmalloc(u32 page_num){
	u32 alloc_length = page_num * 4096;
	u32 return_base = -1;
	struct memory_info_entry *temp_entry = start_entry;
	for(;;){
		//遍历内存描述符链表，寻找空闲内存块
		if(!(temp_entry->length_low > alloc_length)){
			temp_entry = temp_entry->next;
			if((u32)temp_entry != -1)
				continue;
			else
				return return_base;
		}
		
		//新建一个链表项保存该块内存未分配区域，如果剩余的内存区域够大的话
		if((temp_entry->length_low - alloc_length) > sizeof(struct memory_info_entry)){
			struct memory_info_entry *new_entry = (struct memory_info_entry*)(temp_entry->base_addr_low + alloc_length);
			new_entry->base_addr_low = temp_entry->base_addr_low + alloc_length + sizeof(struct memory_info_entry);
			new_entry->length_low = temp_entry->length_low - alloc_length - sizeof(struct memory_info_entry);
			new_entry->mem_type = IN_FREE;
			
			//将新的链表项添加到链表当中
			new_entry->previouse = temp_entry;
			new_entry->next = temp_entry->next;
			temp_entry->next = new_entry;
		}
		//否则将整块内存分配给该申请
		else{
			alloc_length = temp_entry->length_low;
		}
		
		//分配内存并修改该内粗描述符的属性
		return_base = temp_entry->base_addr_low;
		temp_entry->mem_type = IN_USE;
		temp_entry->length_low = alloc_length;
		
		return return_base;
		
		/*switch(mem_info[i].type){
		case ALL_FREE:
			if((mem_info[i].length_low / 2) > alloc_length){
				//填充内存分配信息
				struct mem_free_tree *temp_info1 = (struct mem_free_tree*)base_addr_low;
				temp_info1->base_addr_low = mem_info[i].base_addr_low;
				temp_info1->length_low = size + sizeof(struct mem_free_tree);
				
				mem_info[i].part1 = temp_info;
				mem_info[i].type = ONE_USE;
				
				mem_info[i].part2 = 
			}
			break;
		case ONE_USE:
			break;
		case ALL_USE:
			break;
		}*/
	}
}

bool pfree(u32 base_addr){
	struct memory_info_entry *temp_entry = start_entry;
	for(;;){
		//遍历链表寻找释放的内存块
		if(!(temp_entry->base_addr_low == base_addr)){
			temp_entry = temp_entry->next;
			continue;
			if((u32)temp_entry != -1)
				continue;
			else
				return -1;
		}
		
		//释放内存块
		temp_entry->mem_type = IN_FREE;
		
		//合并空闲内存块
		u8 merge_type = 0;	//只有当前释放块空闲
		struct memory_info_entry *previouse_entry = temp_entry->previouse,*next_entry = temp_entry->next;
		if((u32)previouse_entry != -1&&previouse_entry->mem_type == IN_FREE){
			merge_type = 1;	//前一块空闲
		}
		if((u32)next_entry != -1 && next_entry->mem_type == IN_FREE){
			if(merge_type == 1)
				merge_type = 3;	//前后以及当前三块内存都空闲
			else
				merge_type = 2;	//后一块内存空闲
		}
		switch(merge_type){
		case 1://前一块空闲
			previouse_entry->next = next_entry;
			previouse_entry->length_low += temp_entry->length_low + sizeof(struct memory_info_entry);
			break;
		case 2://后一块空闲
			temp_entry->length_low += next_entry->length_low + sizeof(struct memory_info_entry);
			temp_entry->next = next_entry->next;			
			break;
		case 3://前后以及当前三块内存都空闲
			previouse_entry->length_low += temp_entry->length_low + next_entry->length_low + 2 * sizeof(struct memory_info_entry);
			previouse_entry->next = next_entry->next;
			break;
		}
	}
	return 1;
}

u32 get_free_page(){

}
