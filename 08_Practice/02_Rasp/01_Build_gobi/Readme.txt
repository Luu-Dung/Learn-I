# INFORMATION SUMMARY
- DESCRIPTION: Try to cross-compile and install Qualcomm driver (GobiSerial and GobiNet)
    into Raspberry pi3 
- CHECKING: Check Serial connection and Network connection.

# CONTENT:
- Step1: Install RaspOS by NOOBS
- Step2: Check kernel verion( ex: Linux raspberrypi 5.4.51-v7+ #1333 SMP Mon Aug 10 16:45:19 BST 2020 armv7l GNU/Linux).
- Step3: Download kernel source and toolchain.
            + https://github.com/raspberrypi/linux ( because install RaspOS), check exactly version (by date).
            + git clone https://github.com/raspberrypi/tools ( As an option, we are using toolchain that develop by Rasp's owner, we can use some different toolchain example: linaro, "sudo apt install crossbuild-essential-armhf"(follow rasp guide), ...)
- Step4: clean and build kernel. ( remember install some essential packages, ex: ...)
            + old config option:  Inside kernel directory,
                $ make mrproper
                $ Come to rasp: $sudo modprobe configs -> $zcat /proc/config.gz > /tmp/.config -> copy .config to kernel dir. comeback to kernel DIR.
                $ export CCPREFIX=XXXXXXX/tools/arm-bcm2708/arm-bcm2708-linux-gnueabi/bin/arm-bcm2708-linux-gnueabi-
                $ make ARCH=arm CROSS\_COMPILE=${CCPREFIX} -j 12
- Step5: change PI_KDIR vs PI_CCPREFIX in Makefile (folder GobiSerial and GobiNet).
- Step6: Build and install driver:
            $ make pi (make pi RAW=1 for chip EMxx)
- Step7: Copy output file to rasp, then come to rasp (Remeber: plug EMxx into rasp before insmod).
            $ insmod GobiSerial.ko
            $ insmod GobiNet.ko


# REFER:
- https://techfortalk.co.uk/2020/02/21/cross-compiling-custom-linux-kenel-driver-for-rasberry-pi-platform/
- https://www.raspberrypi.org/documentation/linux/kernel/building.md
- https://techfortalk.co.uk/2020/02/21/unknown-symbols-in-gobiserial-lte-driver/