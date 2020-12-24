			CROSS-COMPILE LINUX KERNEL MODULES 
===============================================================================================

A. CONTENT
I. Prepare Resource
  1. Linux kernel:
	- https://mirrors.edge.kernel.org/pub/linux/kernel/ (General).
	- git clone https://github.com/raspberrypi/linux -> git checkout <version> (Raspberry).
	- git clone git://github.com/beagleboard/kernel.git -> git checkout <version> (Beagle black).
	- so on,...


  2. Toolchain:
	- https://releases.linaro.org/components/toolchain/binaries/ (linaro toolchain).
	- sudo apt install crossbuild-essential-(armhf/amd64) (Raspberry toolchain)
	- sudo apt-get install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf (install directly on ubuntu).
	- so on,...


II. Build process:
  1. Kernel Compile.
	- Get .config from board:  Inside kernel directory,
          $ make clean -> make distclean -> make mrproper 
          $ Come to Board: $sudo modprobe configs -> $zcat /proc/config.gz > /tmp/.config -> copy .config to kernel dir. comeback to kernel DIR.
          $ export CCPREFIX=XXXXXXX/bin/***-linux-gnueabi-
	  $ make menuconfig
          $ make ARCH=arm CROSS\_COMPILE=${CCPREFIX} -j 12 (ARCH=arm: 32bit OS/ amd64: 64bit OS)

  2. prepare your kernel module:
	- Create Makefile: define "Cross compiler" and "Kernel dir" Path.


NOTE:
  Check "uname -m" on Board:
	- Choose excatly kernel version.
	- Choose excatly toolchain (32 or 64bit, half-float or NOT).
	- Find config file ( /proc/config.gz or /boot/config*).
	- Apply config exactly (avoid error module_layout).
	- Choose exactly ARCH.

===============================================================================================
B. REFER:
- General (Refer: https://blukat29.github.io/2017/12/cross-compile-arm-kernel-module/)
- Raspberry (Refer: https://www.raspberrypi.org/documentation/linux/kernel/building.md)
- BeagleBone Black (Refer: https://sites.google.com/site/beagleboneblac/home/cross-compiling-linux-kernel-module)
- https://techfortalk.co.uk/2020/02/21/cross-compiling-custom-linux-kenel-driver-for-rasberry-pi-platform/



===========================================END=================================================
