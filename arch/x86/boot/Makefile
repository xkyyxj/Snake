AS:=as --32 -o
OBJCOPY_OPT = -R .comment -R .note -S -O binary

src := $(obj)

FINAL_BIN := $(src)/boot.bin	#binary process by objcopy
FINAL_O := $(src)/final.o		#binary without objcopy

C_OUT := $(patsubst %.c,%.o,$(wildcard $(src)/*.c))
ASM_OUT := $(patsubst %.S,%.o,$(wildcard $(src)/*.S))

ALL_OUT := $(C_OUT)
ALL_OUT += $(ASM_OUT)

CC :=i686-elf-gcc -c -m16 -nostdlib -O2 -fstrength-reduce

#设置为强制目标，是不是不太好的做法？
PHONY += $(FINAL_BIN) $(FINAL_O)

target : $(FINAL_BIN)
	gcc -o $(src)/final/build.exe $(src)/final/build.c
	$(src)/final/build.exe $(FINAL_BIN) $(srctree)/kernel.elf $(src)/final/kernel.img

$(FINAL_BIN) : $(FINAL_O)
	readelf -a $(FINAL_O) >> $(src)/boot.map
	objcopy $(OBJCOPY_OPT) $(FINAL_O) $(FINAL_BIN)

$(FINAL_O) : $(ALL_OUT)
	echo $(ALL_OUT)
	ld -T$(src)/boot.ld -nostdlib -S -o $(FINAL_O) $(ALL_OUT)

$(C_OUT) : %.o : %.c
	$(CC) -o $@ $<

$(ASM_OUT) : %.o : %.S
	$(AS) $@ $<

.PHONY : clean
clean :
	$(RM) $(src)/*.o
	$(RM) $(src)/*.bin
	$(RM) $(src)/boot.map
	$(RM) $(src)/final/build.exe
	$(RM) $(src)/final/kernel.img
