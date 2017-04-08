OUTPUT_FORMAT("elf32-i386","elf32-i386","elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(kernel_main)

SECTIONS
{
	. = 0x10000;
	.text	:{*.text}
	.data	:{*.data}
}
