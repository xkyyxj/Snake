struct __attribute__((packed)) Elf32_Ehdr{
	u8 e_ident[16];			//elf头标识
	u16 e_type;				//该文件类型（2代表可执行文件）
	u16 e_machine;			//该程序运行的体系结构（3代表 Intel 80386）
	u32 e_version;			//确定文件版本
	u32 e_entry;			//程序的入口地址
	u32 e_poff;				//Program header table于文件中偏移量（字节计）
	u32 e_soff;				//Section header table于文件中偏移量（字节计）
	u32 e_flags;			//Processor-specific flags（于IA32此项为0？）
	u16 e_ehsize;			//elf header大小（字节计）
	u16 e_phentsize;		//Program header table条目字节数
	u16 e_phnum;			//Program header table条目数
	u16 e_shentsize;		//Section header table条目字节数
	u16 e_shnum;			//Section header table条目数
	u16 e_shstrndx;			//包含section名称的字符串表的section索引号（从0计）
};

struct __attribute__((packed)) Elf32_Phdr{
	u32 p_type;				//elf头标识
	u32 p_offset;			//段第一个字节在文件中的偏移
	u32 p_vaddr;			//段第一个字节于内存中的虚拟地址
	u32 p_paddr;			//在物理内存定位相关的系统中，此项是为物理地址保留
	u32 p_filesz;			//段在文件中的长度
	u32 p_memsz;			//段在内存中的长度
	u32 p_flags;			//与段相关的标志
	u32 p_align;			//根据此项值来确定段在文件及内存中如何对齐
};