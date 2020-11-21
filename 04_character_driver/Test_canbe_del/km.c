#include <linux/module.h>
#include <linux/fs.h> // Define allocate/free device number Function!
#include <linux/device.h>
#include <linux/slab.h> 
#include <linux/cdev.h>

#include "dev_info.h"

// Description:
#define DRIVER_AUTHOR "NHATMT"
#define DRIVER_DESC "Draft"

/************* Structure Specific ****************/
typedef struct hw_struct{
    char *sts_reg;
    char *ctrl_reg;
    char *data_reg;
} hw_str_t;

struct drv_stuct{
    dev_t dev_num;
    struct class *dev_class;
	struct device *dev;
    hw_str_t * hw_reg;
    struct cdev *dev_cdev;
	unsigned int open_cnt;
} driver_str;



/************* Device Specific ****************/
static int hw_init(hw_str_t *dev_hw){

    // Gia lap physical address cho device
    char *hwbuf;
    hwbuf = kzalloc(NUM_REG*REG_SIZE, GFP_KERNEL);
    if(hwbuf == NULL){
        return -ENOMEM;
    }

    dev_hw->sts_reg = hwbuf;
    dev_hw->ctrl_reg = hwbuf + NUM_STS_REG;
    dev_hw->data_reg = dev_hw->ctrl_reg + NUM_CTRL_REG;

    // Config ban dau cho device
    dev_hw->sts_reg = 0x000;
    dev_hw->ctrl_reg = 0x001;

    return 0;
}

static void hw_exit(hw_str_t *dev_hw){
    kfree(dev_hw->sts_reg);
}

/************* OS Specific ****************/
/* cac ham entry points */
// Open
static int driver_open(struct inode *inode, struct file *flip){
	driver_str.open_cnt++;
	printk("Handle opened event (%d)\n", driver_str.open_cnt);
	return 0;
}

// Close
static int driver_release(struct inode *inode, struct file *flip){
	printk("Handle closed event\n");
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_release,
};


static int __init km_driver_init(void){
    int ret = 0;

    /* 1. cap phat device number */
	driver_str.dev_num = 0;
	ret = alloc_chrdev_region(&driver_str.dev_num,0,1, "Name_in_proc/device");
	if (ret < 0){
		printk("failed to register device number\n");
		goto failed_register_devnum;
	}
	printk("allocated device number (%d,%d)\n", MAJOR(driver_str.dev_num), MINOR(driver_str.dev_num));

    // create dev class folder in /sys/class
    driver_str.dev_class = class_create(THIS_MODULE, "class_km_dev"); // CONFIRMED
	if(driver_str.dev_class == NULL){
		printk("failed to create device class\n");
		goto fail_create_devclass;
    }

    // create dev file in /proc/devices (chua xh trong /dev)
    driver_str.dev = device_create(driver_str.dev_class, NULL, driver_str.dev_num, NULL, "Name_in_dev"); // CONFIRMED
    if(driver_str.dev == NULL){
		printk("failed to create device file\n");
		goto fail_create_devfile;
    }

    // Gia lap phan cung de tuong tac
    driver_str.hw_reg = kzalloc(sizeof(hw_str_t), GFP_KERNEL);
    if(driver_str.hw_reg == NULL){
        printk("Allocate -> gia lap hw fail\n");
        goto fail_create_hw_reg;
    }

    ret = hw_init(driver_str.hw_reg);
    if(ret<0){
        printk("Allocate -> gia lap hw fail\n");
        goto fail_create_hw;
    }

    /* 5. dang ky cac entry point voi kernel */
	driver_str.dev_cdev = cdev_alloc();
	if (driver_str.dev_cdev == NULL){
		printk("failed to allocate cdev structure\n");
		goto failed_allocate_cdev;
	}
	cdev_init(driver_str.dev_cdev, &fops);
	ret = cdev_add(driver_str.dev_cdev, driver_str.dev_num, 1);
	if(ret < 0){
		printk("failed to add a char device to the system\n");
		goto failed_allocate_cdev;
	}

    printk("Module intialized!\r\n");
    return 0;


//FAIL
failed_allocate_cdev:
    hw_exit(driver_str.hw_reg);
fail_create_hw:
    kfree(driver_str.hw_reg);
fail_create_hw_reg:
    device_destroy(driver_str.dev_class, driver_str.dev_num);
fail_create_devfile:
    class_destroy(driver_str.dev_class);
fail_create_devclass:
    unregister_chrdev_region(driver_str.dev_num,1);
failed_register_devnum:
	return ret;
}


static void __exit km_driver_exit(void){

    // Xoa cdev
    cdev_del(driver_str.dev_cdev);

    // Xoa hw_reg gia lap
    hw_exit(driver_str.hw_reg);
    kfree(driver_str.hw_reg);

    /* xoa bo device file */
	device_destroy(driver_str.dev_class, driver_str.dev_num);
	class_destroy(driver_str.dev_class);

	/* giai phong device number */
	unregister_chrdev_region(driver_str.dev_num,1);

    printk("Module De-intialized!\r\n");
}




/************* Action Specific ****************/
module_init(km_driver_init);
module_exit(km_driver_exit);

MODULE_LICENSE("GPL"); /* giay phep su dung cua module */
MODULE_AUTHOR(DRIVER_AUTHOR); /* tac gia cua module */
MODULE_DESCRIPTION(DRIVER_DESC); /* mo ta chuc nang cua module */
//MODULE_VERSION(DRIVER_VERSION); /* mo ta phien ban cuar module */
MODULE_SUPPORTED_DEVICE("testdevice"); /* kieu device ma module ho tro */