AS=as --32 -o
OBJCOPY_OPT = -R .comment -R .note -S -O binary
FINAL_BIN = boot.bin	#binary process by objcopy
FINAL_O = final.o		#binary without objcopy

C_OUT := $(patsubst %.c,%.o,$(wildcard *.c))
ASM_OUT := $(patsubst %.S,%.o,$(wildcard *.S))

ALL_OUT := $(C_OUT)
ALL_OUT += $(ASM_OUT)

CC =i686-elf-gcc -c -m16 -nostdlib -O2 -fstrength-reduce

$(FINAL_BIN) : $(FINAL_O)
	objcopy $(OBJCOPY_OPT) $(FINAL_O) $(FINAL_BIN)

$(FINAL_O) : $(ALL_OUT)
	echo $(ALL_OUT)
	ld -Tboot.ld -nostdlib -S -o $(FINAL_O) $(ALL_OUT)

$(C_OUT) : %.o : %.c
	$(CC) -o $@ $<
	
$(ASM_OUT) : %.o : %.S
	$(AS) $@ $<

.PHONY : clean

clean :
	-rm *.o
	-rm *.bin
