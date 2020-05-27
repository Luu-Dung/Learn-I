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
#define DRIVER_AUTHOR "Nhat Le <nhatmt95@gmail.com>"
#define DRIVER_DESC   "A sample character device driver"
#define DRIVER_VERSION "0.4"
#define Dynamically

/****************************** Driver Structs *****************************/

struct _vchar_drv {
	dev_t dev_num;
	struct class *dev_class;
	struct device *dev;
} vchar_drv;




/****************************** device specific - START *****************************/
/* ham khoi tao thiet bi */

/* ham giai phong thiet bi */

/* ham doc tu cac thanh ghi du lieu cua thiet bi */

/* ham ghi vao cac thanh ghi du lieu cua thiet bi */

/* ham doc tu cac thanh ghi trang thai cua thiet bi */

/* ham ghi vao cac thanh ghi dieu khien cua thiet bi */

/* ham xu ly tin hieu ngat gui tu thiet bi */

/******************************* device specific - END *****************************/

/******************************** OS specific - START *******************************/
/* cac ham entry points */

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

	/* 4. khoi tao thiet bi vat ly */

	/* 5. dang ky cac entry point voi kernel */

	/* 6. dang ky ham xu ly ngat */

	printk("Initialize vchar driver successfully\n");
	return 0;
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

	/* giai phong thiet bi vat ly */

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
