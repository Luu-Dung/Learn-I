#Makefile

obj-m += km.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean


# Compile with different kernel version
#all:
#	make -C /lib/modules/3.18.140-r1/build M=$(PWD) modules
#clean:
#	make -C /lib/modules/kernel-headers-3.18.140/build M=$(PWD) clean
