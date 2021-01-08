# Cross compile
obj-m += usb_driver.o

PWD := $(shell pwd)
#KERNEL_DIR  := '../01_ToolcvsKernel/linux-raspberrypi-kernel_1.20201201-1' 
KERNEL_DIR  := '../01_ToolcvsKernel/linux-raspberrypi-kernel_1.20201126-1'
#KERNEL_DIR  := '../01_ToolcvsKernel/linux-raspberrypi-kernel_1.20200811-1'
#CROSS_COMP := '/media/ole/DATA/05_Practice/02_Learn-I/08_Practice/02_Rasp/03_Linux_device_driver/01_ToolcvsKernel/tools/arm-bcm2708/arm-bcm2708-linux-gnueabi/bin/arm-bcm2708-linux-gnueabi-'
#CROSS_COMP := /media/ole/DATA/05_Practice/02_Learn-I/08_Practice/02_Rasp/03_Linux_device_driver/01_ToolcvsKernel/tools-master/arm-bcm2708/arm-bcm2708-linux-gnueabi/bin/arm-bcm2708-linux-gnueabi- 
#CROSS_COMP := /media/ole/DATA/05_Practice/03_Cross-compile/00_Source/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
CROSS_COMP := arm-linux-gnueabihf-


pi:
	make ARCH=arm CROSS_COMPILE=$(CROSS_COMP) -C $(KERNEL_DIR) M=$(PWD) modules
	#$(MAKE) ARCH=arm CROSS_COMPILE=${CROSS_COMP} -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	#$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
	make -C $(KERNEL_DIR)  M=$(shell pwd) clean
