#体系结构（暂时好像用不到了，希望不会永远用不到）
ARCH := x86
export ARCH

#规定相应的全局都可用的变量(子Makefile可能会重新定义)
CC := i686-elf-gcc -c -nostdlib -O2 -fstrength-reduce -o
AS := as -o
LD := ld -r -S -nostdlib -o
export CC
export AS
export LD
FINAL_LD := ld

PHONY := all
all :

#编译的根目录
srctree := .
export srctree

#当前的根目录
src := $(srctree)

build := -f $(srctree)/Makefile.build obj

#定义需要编译的子目录
dirs := drivers/

include $(srctree)/Makefile.lib

#需构建进自子内核的模块
core-y := $(addsuffix /build-in.o,$(subdirs))

all : final

#构建最终的内核二进制映像，最后处理与体系结构相关的内容
PHONY += final
final : kernel
	$(Q)$(MAKE) $(build)=arch/$(ARCH)

#内核主体(强制执行，依赖一个强制目标)
kernel : $(subdirs) FORCE
	$(LD) kernel $(core-y)

#构建各个子目录
PHONY += $(subdirs)
$(subdirs) :
	$(Q)$(MAKE) $(build)=$@

#强制目标
FORCE :

clean-param := -f $(srctree)/Makefile.clean obj

PHONY += $(clean-subdirs)
clean-subdirs := $(addprefix __clean__,$(subdirs))

PHONY += clean
clean : $(clean-subdirs)
	$(Q)$(MAKE) $(clean-param)=$(srctree)/arch/$(ARCH)
	$(RM) kernel

$(clean-subdirs) :
	$(Q)$(MAKE) $(clean-param)=$(patsubst __clean__%,%,$@)

.PHONY : $(PHONY)
