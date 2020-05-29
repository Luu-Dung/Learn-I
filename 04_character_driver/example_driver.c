/*
 * ten file: vchar_driver.c
 * tac gia : dat.a3cbq91@gmail.com
 * ngay tao: 9/12/2018
 * mo ta   : char driver cho thiet bi gia lap vchar_device.
 *           vchar_device la mot thiet bi nam tren RAM.
 */

#include <linux/module.h> /* thu vien nay dinh nghia cac macro nhu module_init va module_exit */
#include <linux/fs.h> // Define allocate/free device number Function!
#include <linux/device.h> // Provide the creating device file function.
#include <linux/slab.h> // Provide the kmalloc & free function
#include <linux/cdev.h> // Provide cdev's function.
#include <linux/uaccess.h> // Provide comunicate between user and kernel.

#include "vchar_device.h" // Device register description

#define DRIVER_AUTHOR "Nhat Le <nhatmt95@gmail.com>"
#define DRIVER_DESC   "A sample character device driver"
#define DRIVER_VERSION "0.7"
#define Dynamically

/****************************** Driver Structs *****************************/
// Hardware struct
typedef struct vchar_dev {
	unsigned char * control_regs;
	unsigned char * status_regs;
	unsigned char * data_regs;
} vchar_dev_t;



// OS struct
struct _vchar_drv {
	dev_t dev_num;
	struct class *dev_class;
	struct device *dev;
	vchar_dev_t *vchar_hw;
	struct cdev *vcdev;
	unsigned int open_cnt;
} vchar_drv;




/****************************** device specific - START *****************************/
/* A. ham khoi tao thiet bi */
int vchar_hw_init(vchar_dev_t *hw){
	char *buf;
	buf = kzalloc(NUM_DEV_REGS*REG_SIZE, GFP_KERNEL);
	if(!buf){
		return -ENOMEM;
	}

	hw->control_regs = buf;
	hw->status_regs = hw->control_regs + NUM_CTRL_REGS;
	hw->data_regs = hw->status_regs + NUM_STS_REGS;

	// khoi tao gia tri ban dau cho cac thanh ghi
	hw->control_regs[CONTROL_ACCESS_REG] = 0x03;
	hw->status_regs[DEVICE_STATUS_REG] = 0x03;



	return 0;
}

/* B. ham giai phong thiet bi */
void vchar_hw_exit(vchar_dev_t *hw){
	kfree(hw->control_regs);
}

/* C. ham doc tu cac thanh ghi du lieu cua thiet bi */
static int vchar_hw_read_data(vchar_dev_t *hw, int start_regs, int num_regs, char *kbuf){
	int read_bytes = num_regs*REG_SIZE;

	// Check reading permission.
	if((hw->control_regs[CONTROL_ACCESS_REG]&CTRL_READ_DATA_BIT) == DISABLE){
		return -1;
	}

	// check kernel buffer.
	if(!kbuf){
		return -1;
	}

	// check the validity of the start register.
	if(start_regs > NUM_DATA_REGS){
		return -1;
	}

	// Adjust number of reading register.
	if(num_regs > (NUM_DATA_REGS - start_regs)){
		read_bytes = (NUM_DATA_REGS - start_regs)*REG_SIZE;
	}

	// Copy data from data_regs to kernel buffer.
	/* Do char device của chúng ta là một thiết bị giả lập trên RAM, 
	nên ta chỉ cần dùng hàm memcpy là đã có thể đọc dữ liệu từ buffer của 
	char device lên kernel buffer.
	 Nếu char device của chúng ta là một thiết bị I2C, chúng ta phải dùng hàm khác như i2c_read_byte chẳng hạn...*/
	memcpy(kbuf,hw->data_regs+start_regs, read_bytes);

	// Update number of read data_reg.
	hw->status_regs[READ_COUNT_L_REG] +=1;
	if(hw->status_regs[READ_COUNT_L_REG] == 0){
		hw->status_regs[READ_COUNT_H_REG] +=1;
	}

	// RETURN read bytes
	return read_bytes;
}


/* D. ham ghi vao cac thanh ghi du lieu cua thiet bi */
static int vchar_hw_write_data(vchar_dev_t *hw, int start_reg, int num_regs, char *kbuf){
	int write_byte = num_regs;

	// check write permission.
	if((hw->control_regs[CONTROL_ACCESS_REG]&CTRL_WRITE_DATA_BIT) == DISABLE){
		return -1;
	}

	// check kernel buffer.
	if(!kbuf){
		return -2;
	}

	// check the validity of registers writen.
	if(num_regs > (NUM_DATA_REGS - start_reg)){
		write_byte = NUM_DATA_REGS - start_reg;
		hw->status_regs[DEVICE_STATUS_REG] |= STS_DATAREGS_OVERFLOW_BIT;
	}

	// Copy data from kernel buffer to data_regs of device.
	memcpy(hw->data_regs+start_reg, kbuf, write_byte);

	// Update number of writing register.
	hw->status_regs[WRITE_COUNT_L_REG] +=1;
	if(hw->status_regs[WRITE_COUNT_L_REG] == 0){
		hw->status_regs[WRITE_COUNT_H_REG] +=1;
	}

	// Return number of writing byte.
	return write_byte;
}
/* E. ham doc tu cac thanh ghi trang thai cua thiet bi */

/* F. ham ghi vao cac thanh ghi dieu khien cua thiet bi */

/* G. ham xu ly tin hieu ngat gui tu thiet bi */

/******************************* device specific - END *****************************/




/******************************** OS specific - START *******************************/
/* cac ham entry points */
static int vchar_driver_open(struct inode *inode, struct file *flip){
	vchar_drv.open_cnt++;
	printk("Handle opened event (%d)\n", vchar_drv.open_cnt);
	return 0;
}

static int vchar_driver_release(struct inode *inode, struct file *flip){
	printk("Handle closed event\n");
	return 0;
}

static ssize_t vchar_driver_read(struct file *filp, char __user *user_buf, size_t len, loff_t *off)
{
	char *kernel_buf = NULL;
	int num_bytes = 0;

	printk("Handle read event start from %lld, %zu bytes\n", *off, len);

	kernel_buf = kzalloc(len, GFP_KERNEL);
	if(!kernel_buf){
		return 0;
	}

	num_bytes = vchar_hw_read_data(vchar_drv.vchar_hw, *off, len, kernel_buf);
	printk("read %d bytes from HW\n", num_bytes);

	if(num_bytes < 0){
		return -EFAULT;
	}

	if(copy_to_user(user_buf, kernel_buf, num_bytes)){
		return -EFAULT;
	}

	*off +=num_bytes;
	return num_bytes;
}

static ssize_t vchar_driver_write(struct file *filp, const char __user *user_buf, size_t len, loff_t *off)
{
	char *kernel_buf = NULL;
	int num_bytes = 0;
	printk("Handle write envent start from %lld, %zu bytes\n", *off, len);

	kernel_buf = kzalloc(len, GFP_KERNEL);
	if(copy_from_user(kernel_buf, user_buf, len)){
		return -EFAULT;
	}

	num_bytes = vchar_hw_write_data(vchar_drv.vchar_hw, *off, len, kernel_buf);
	printk("writes %d bytes to HW\n", num_bytes);

	if(num_bytes <0){
		return -EFAULT;
	}

	*off += num_bytes;
	return num_bytes;
}


static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = vchar_driver_open,
	.release = vchar_driver_release,
	.read = vchar_driver_read,
	.write = vchar_driver_write,

};

/* ham khoi tao driver */
static int __init vchar_driver_init(void)
{
	int ret = 0;

	/* 1. cap phat device number */
#ifdef Dynamically
	vchar_drv.dev_num = 0;
	ret = alloc_chrdev_region(&vchar_drv.dev_num,0,1, "Name_device");
#else /* Statically */
	vchar_drv.dev_num = MKDEV(235,0);
	ret = register_chrdev_region(vchar_drv.dev_num,1, "Name_device");
#endif

	if (ret < 0){
		printk("failed to register device number\n");
		goto failed_register_devnum;
	}
	printk("allocated device number (%d,%d)\n", MAJOR(vchar_drv.dev_num), MINOR(vchar_drv.dev_num));
	
	/* 2. tao device file */
	/* Create Tạo ra một lớp các thiết bị
	(thu muc) có tên là [name] trong
 		thư mục /sys/class. Lớp này chứa liên kết tới thông
		tin của các thiết bị cùng loại. */
	vchar_drv.dev_class = class_create(THIS_MODULE, "class_vchar_dev");
	if(vchar_drv.dev_class == NULL){
		printk("failed to create device class\n");
		goto failed_create_class;
	}
	/* Tao ra các thông tin của một thiết bị cụ thể.
		Khi có thông tin này, udev sẽ tạo ra một device file
		tương ứng trong /dev */
	vchar_drv.dev = device_create(vchar_drv.dev_class, NULL, vchar_drv.dev_num, NULL, "vchar_dev");
	if(IS_ERR(vchar_drv.dev)){
		printk("faild to create a divice\n");
		goto failed_create_device;
	}

	/* 3. cap phat bo nho cho cac cau truc du lieu cua driver va khoi tao */
	vchar_drv.vchar_hw = kzalloc(sizeof(vchar_dev_t), GFP_KERNEL);
	if(!vchar_drv.vchar_hw){
		printk("failed to alloocate data structure for hardware\n");
		ret = -ENOMEM;
		goto failed_allocate_structure;
	}

	/* 4. khoi tao thiet bi vat ly */
	ret = vchar_hw_init(vchar_drv.vchar_hw);
	if(ret < 0){
		printk("failed to initialize a virtual character device\n");
		goto failed_init_hw;
	}
	/* 5. dang ky cac entry point voi kernel */
	vchar_drv.vcdev = cdev_alloc();
	if (vchar_drv.vcdev == NULL){
		printk("failed to allocate cdev structure\n");
		goto failed_allocate_cdev;
	}
	cdev_init(vchar_drv.vcdev, &fops);
	ret = cdev_add(vchar_drv.vcdev, vchar_drv.dev_num, 1);
	if(ret < 0){
		printk("failed to add a char device to the system\n");
		goto failed_allocate_cdev;
	}
	/* 6. dang ky ham xu ly ngat */

	printk("Initialize vchar driver successfully\n");
	return 0;
failed_allocate_cdev:
	vchar_hw_exit(vchar_drv.vchar_hw);
failed_init_hw:
	kfree(vchar_drv.vchar_hw);
failed_allocate_structure:
	device_destroy(vchar_drv.dev_class, vchar_drv.dev_num);
failed_create_device:
	class_destroy(vchar_drv.dev_class);
failed_create_class:
	unregister_chrdev_region(vchar_drv.dev_num, 1);
failed_register_devnum:
	return ret;


}

/* ham ket thuc driver */
static void __exit vchar_driver_exit(void)
{
	/* huy dang ky xu ly ngat */

	/* huy dang ky entry point voi kernel */
	cdev_del(vchar_drv.vcdev);

	/* giai phong thiet bi vat ly */
	vchar_hw_exit(vchar_drv.vchar_hw);
	/* giai phong bo nho da cap phat cau truc du lieu cua driver */

	/* xoa bo device file */
	device_destroy(vchar_drv.dev_class, vchar_drv.dev_num);
	class_destroy(vchar_drv.dev_class);

	/* giai phong device number */
	unregister_chrdev_region(vchar_drv.dev_num,1);

	printk("Exit vchar driver\n");
}
/********************************* OS specific - END ********************************/

module_init(vchar_driver_init);
module_exit(vchar_driver_exit);

MODULE_LICENSE("GPL"); /* giay phep su dung cua module */
MODULE_AUTHOR(DRIVER_AUTHOR); /* tac gia cua module */
MODULE_DESCRIPTION(DRIVER_DESC); /* mo ta chuc nang cua module */
MODULE_VERSION(DRIVER_VERSION); /* mo ta phien ban cuar module */
MODULE_SUPPORTED_DEVICE("testdevice"); /* kieu device ma module ho tro */
