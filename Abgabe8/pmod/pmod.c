#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/fdtable.h>
#include <asm/atomic.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Walter Jensch");
MODULE_DESCRIPTION("Shows information about child processes for a given PID");
MODULE_PARM_DESC(pid, "PID of the process to analyze");

static int pid = 1;

module_param(pid, int, 0000);

atomic_t fileCount;
struct files_struct *fs;

static int __init pmod_init(void) {
	struct task_struct *ts;

	printk(KERN_INFO "[pmod] loaded");


	for_each_process(ts) {

		if(ts->parent->pid != pid) {
			continue;
		}

		fs = ts->files;
		fileCount = fs->count;
			
		printk(KERN_INFO "Prozess ID: %d, Prozess Status: %ld, User ID: %d, Open Files: %d, Time Userspace: %ld, Time Kernelspace: %ld\n", ts->pid, ts->state, ts->cred->uid.val, atomic_read(&fileCount), ts->utime, ts->stime);
	}
	
	
	printk(KERN_INFO "%d", ts->pid);
	return 0;
}

static void __exit pmod_exit(void) {
	printk(KERN_INFO "[pmod] unloaded");
}

module_init(pmod_init);
module_exit(pmod_exit);
