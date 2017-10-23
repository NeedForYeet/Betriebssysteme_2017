#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/export.h>

#define DEV_NAME "namedev"
#define MAJOR_NUMBER 120
#define MINOR_NUMBER 100

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Walter Jensch");
MODULE_DESCRIPTION("Driver for a simple character device.");

dev_t namedev;
struct cdev *namedev_cdev;
struct file_operations namedev_fops = {
	.owner = THIS_MODULE,
};

static int __init namedev_init(void) {


	namedev = MKDEV(MAJOR_NUMBER, MINOR_NUMBER);


	if (register_chrdev_region(namedev, 1, DEV_NAME) != 0) {
		return -1;
	}

	namedev_cdev = cdev_alloc();
	namedev_cdev->ops = &namedev_fops;

	cdev_add(namedev_cdev, namedev, 1);

	printk(KERN_INFO "[namedev]: loaded");
	return 0;
}

static void __exit namedev_exit(void) {
	printk(KERN_INFO "[namedev]: unloaded");
	unregister_chrdev_region(namedev, 1);
	cdev_del(namedev_cdev);
}

module_init(namedev_init);
module_exit(namedev_exit);
