#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/export.h>
#include <linux/uaccess.h>

#define DEV_NAME "namedev"
#define MAJOR_NUMBER 120
#define MINOR_NUMBER 100
#define AUTHOR_NAME "Walter Jensch "

MODULE_LICENSE("GPL");
MODULE_AUTHOR(AUTHOR_NAME);
MODULE_DESCRIPTION("Driver for a simple character device that copies output to the user.");

ssize_t namedev_read(struct file *f, char __user *buf, size_t num, loff_t *offset);
int namedev_open(struct inode *i, struct file *f);


static char name[] = AUTHOR_NAME;
static int nameOffset;
static int nameLength;
dev_t namedev;
struct cdev *namedev_cdev;
struct file_operations namedev_fops = {
	.owner = THIS_MODULE,
	.open = namedev_open,
	.read = namedev_read,
};

// TODO: Properly implement open and read.
// This works, but doesn't use all the structs mentioned in LDD3

int namedev_open(struct inode *i, struct file *f) {
	printk(KERN_INFO "namedev: open\n");

	nameOffset = 0;
	nameLength = sizeof(name)/sizeof(char);

	printk(KERN_INFO "[namedev]: open; Offset: %d, NameLength: %d\n", nameOffset, nameLength);

	return 0;
}

ssize_t namedev_read(struct file *f, char __user *buf, size_t num, loff_t *offset) {
	printk(KERN_INFO "namedev: read %lu, %lld", (unsigned long)num, *offset);

	// copy data to user
	if (copy_to_user(buf, name + nameOffset, 1) != 0) {
		printk(KERN_INFO "[namedev]: copy_to_user() failed\n");
		return -1;
	}

	// increment offset until length -1, then start at 0 again
	nameOffset = (nameOffset + 1) % nameLength;

	return 1;
}

static int __init namedev_init(void) {

	printk(KERN_INFO "namedev: author: %s\n", name);

	namedev = MKDEV(MAJOR_NUMBER, MINOR_NUMBER);


	if (register_chrdev_region(namedev, 1, DEV_NAME) != 0) {
		return -1;
	}

	namedev_cdev = cdev_alloc();
	namedev_cdev->ops = &namedev_fops;

	cdev_add(namedev_cdev, namedev, 1);

	printk(KERN_INFO "[namedev]: loaded\n");
	return 0;
}

static void __exit namedev_exit(void) {
	printk(KERN_INFO "[namedev]: unloaded\n");
	unregister_chrdev_region(namedev, 1);
	cdev_del(namedev_cdev);
}

module_init(namedev_init);
module_exit(namedev_exit);
