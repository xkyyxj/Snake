#此前会将boot目录包进来，但是这样的话，还没有在该目录下生成build-in.o,导致boot目录下
#编译错误
dirs := kernel/

#最终进入boot目录构建启动引导模块，注意此处变换了编译目录
#target :
#	$(Q)$(MAKE) -C $(src)/boot
target :
	$(Q)$(MAKE) -f $(src)/boot/Makefile obj=$(src)/boot
clean :
	$(Q)$(MAKE) -f $(src)/boot/Makefile obj=$(src)/boot clean
	$(RM) $(src)/*.o
