# Cross compile
PWD := $(shell pwd)
obj-m += hello.o

KERNEL_DIR  := '../../01_ToolcvsKernel/linux-3.18.140'
CROSS_COMP := '/media/tma/my_label/00_Practice/Cross-compile/01_ToolcvsKernel/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-'
KERNEL_yocto_DIR := '/WS/wp76/build_src/tmp/work-shared/swi-mdm9x28-wp/kernel-build-artifacts' #/WS/wp76/build_src/tmp/work/swi_mdm9x28_wp-poky-linux-gnueabi/linux-quic/3.18.140-r1/build'

all:
	make ARCH=arm CROSS_COMPILE=$(CROSS_COMP) -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules

yocto:
	make ARCH=arm CROSS_COMPILE=$(CROSS_COMP) -C $(KERNEL_yocto_DIR) SUBDIRS=$(PWD) modules

clean:
	 make -C $(KERNEL_yocto_DIR) SUBDIRS=$(PWD) clean
