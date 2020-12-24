# Cross compile
obj-m += usb_driver.o

PWD := $(shell pwd)
KERNEL_DIR  := '../../01_ToolcvsKernel/linux-4.1.10'
CROSS_COMP := '../../01_ToolcvsKernel/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-'


all:
	make ARCH=arm CROSS_COMPILE=$(CROSS_COMP) -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules
clean:
	 make -C $(KERNEL_DIR) SUBDIRS=$(PWD) clean
