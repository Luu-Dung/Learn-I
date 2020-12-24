# Cross compile
obj-m += usb_driver.o

PWD := $(shell pwd)
#KERNEL_DIR  := ../../01_ToolcvsKernel/linux-4.1.10
#CROSS_COMP := ../../01_ToolcvsKernel/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-

WP76_KDIR := '/WS/wp76/build_src/tmp/work-shared/swi-mdm9x28-wp/kernel-build-artifacts'
WP76_COMP := '/media/tma/my_label/00_Practice/Cross-compile/01_ToolcvsKernel/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-'
			  #/media/tma/my_label/00_Practice/Cross-compile/01_ToolcvsKernel/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-

#all:
	#make ARCH=arm CROSS_COMPILE=$(CROSS_COMP) -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules

wp76:
	make ARCH=arm CROSS_COMPILE=$(WP76_COMP) -C $(WP76_KDIR) SUBDIRS=$(PWD) modules

clean:
	 make -C $(WP76_KDIR) SUBDIRS=$(PWD) clean
