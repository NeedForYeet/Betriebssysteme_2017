#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Walter Jensch");
MODULE_DESCRIPTION("Simple module to print a string to KERN_INFO");
MODULE_PARM_DESC(input, " String to print in dmesg");

static char* input = "world";

module_param(input, charp, 0000);

static int __init hellokernel_init(void) {
	printk(KERN_INFO "Hello %s\n", input);
	
	return 0; // module won't load on non-zero return value.
}

static void __exit hellokernel_exit(void) {
	printk(KERN_INFO "Goodbye, %s\n", input);
}

module_init(hellokernel_init);
module_exit(hellokernel_exit);


