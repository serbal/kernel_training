#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kthread.h>
#include <linux/err.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/completion.h>
#include <linux/rwsem.h>

MODULE_AUTHOR("Sergii Balanets");
MODULE_DESCRIPTION("Multiple Thread module in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

#define IT_NUM 50

static int count;
static int concurr_check;
static struct completion complete_write;


struct rw_semaphore my_rwlock;


static int write_thread(void *data)
{
	int i;

	for (i = 0; i < IT_NUM; i++) {
		down_write(&my_rwlock);
		count++;
		up_write(&my_rwlock);
		mdelay(1);

	}
	complete(&complete_write);
	return 0;
}

static int read_thread1(void *data)
{
	int n;
	while(true) {
		down_read(&my_rwlock);
		concurr_check++;
		n = count;
		mdelay(1);
		concurr_check++;
		up_read(&my_rwlock);
		if (n >= IT_NUM) break;
		printk(KERN_EMERG "reader1: %i", n);
	}
	return 0;
}

static int read_thread2(void *data)
{
	int n;
	while(true) {
		down_read(&my_rwlock);
		if (concurr_check%2)
			printk(KERN_EMERG ">>>>>>>>>>>>>>>>concurrent reading<<<<<<<<<<<<<<<\n");
		n = count;
		mdelay(1);
		up_read(&my_rwlock);
		if (n >= IT_NUM) break;
		printk(KERN_EMERG "reader2: %i", n);
	}
	return 0;
}

static int __exit sync_init(void)
{
	init_rwsem(&my_rwlock); 
	static struct task_struct *kthread_1;
	static struct task_struct *kthread_2;
	static struct task_struct *kthread_3;


	count = 0;
	concurr_check = 0;
	init_completion(&complete_write);
	kthread_1 = kthread_run(write_thread, &count, "write_thread");
	if (IS_ERR(kthread_1)) {
		printk(KERN_EMERG "Unable to start kernel thread 1.\n");
		return 0;
	}

	// init_completion(&complete_th2);
	kthread_2 = kthread_run(read_thread1, &count, "read_thread1");
	if (IS_ERR(kthread_2)) {
		printk(KERN_EMERG "Unable to start kernel read thread 1.\n");
		return 0;
	}
	kthread_3 = kthread_run(read_thread2, &count, "read_thread2");
	if (IS_ERR(kthread_3)) {
		printk(KERN_EMERG "Unable to start kernel read thread 2.\n");
		return 0;
	}
	wait_for_completion(&complete_write);
	printk(KERN_EMERG "count: %i\n", count);
	return 0;
}

static void __exit sync_exit(void)
{

}


module_init(sync_init);
module_exit(sync_exit);
