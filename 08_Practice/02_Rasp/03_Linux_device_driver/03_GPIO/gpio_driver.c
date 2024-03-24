/***************************************************************************//**
*  \file       gpio_driver.c
*
*  \details    Simple gpio driver explanation
*
*  \author     EmbeTronicX
*
*  \Tested with kernel Linux raspberrypi 5.4.79-v7+
*
*******************************************************************************/
#include <linux/kernel.h>
#include <linux/module.h> /* thu vien nay dinh nghia cac macro nhu module_init va module_exit */

#define DEVICE_NAME "gpio_dev"

static unsigned PORT = 0x20200000;
static unsigned RANGE =  0x40;
int Major,cmd;


static ssize_t device_read(struct file *filp,  char *buffer,  size_t length, loff_t * offset);
static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off);
static int device_open(struct inode *inode, struct file *file);
static int device_release(struct inode *inode, struct file *file);

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = device_open,
    .release = device_release,
    .read    = device_read,
    .write   = device_write
};

static int device_open(struct inode *inode, struct file *file)
{
    addr = ioremap(PORT, RANGE);
    cmd = 0;
    writel(cmd, (addr+4));

    printk("Open gpio port successfully!\n");
}


static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
    cmd = 1 << 10;
    writel(cmd, (addr+0x1c));

}


static int __init drv_gpio_init(void)
{
    //register the USB device
    Major = register_chrdev(0,DEVICE_NAME, &fops);

    check_mem_region(PORT, RANGE);
    request_mem_region(PORT, RANGE, DEVICE_NAME);
    cmd = 0;
    writel(cmd, (addr+4));

    return 0;
}

static void __exit drv_gpio_exit(void)
{
    //deregister the USB device
    release_mem_region(PORT, RANGE);
    unregister_chrdev(Major, DEVICE_NAME);
}

module_init(drv_gpio_init);
module_exit(drv_gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("OleV <nhamt95@gmail.com>");
MODULE_DESCRIPTION("A simple device driver - GPIO Driver");
MODULE_VERSION("1.0");