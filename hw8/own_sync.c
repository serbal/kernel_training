#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kthread.h>
#include <linux/err.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/completion.h>


MODULE_AUTHOR("Sergii Balanets");
MODULE_DESCRIPTION("Multiple Thread module in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static int count;
static struct completion complete_th1, complete_th2;
static atomic_t flg;

static void lock(void)
{
	while (atomic_xchg(&flg, 1))
		ndelay(1);
}

static void unlock(void)
{
	atomic_xchg(&flg, 0);
}

static int thread1_func(void *data)
{
	int i;

	for (i = 0; i < 1000000; i++) {
		lock();
		count++;
		unlock();
		ndelay(1);
	}
	complete(&complete_th1);
	return 0;
}

static int thread2_func(void *data)
{
	int i;

	for (i = 0; i < 1000000; i++) {
		lock();
		count++;
		unlock();
		ndelay(1);
	}
	complete(&complete_th2);
	return 0;
}

static int __exit sync_init(void)
{
	static struct task_struct *kthread_1;
	static struct task_struct *kthread_2;

	count = 0;
	init_completion(&complete_th1);
	kthread_1 = kthread_run(thread1_func, &count, "test_tread");
	if (IS_ERR(kthread_1)) {
		printk(KERN_EMERG "Unable to start kernel thread 1.\n");
		return 0;
	}

	init_completion(&complete_th2);
	kthread_2 = kthread_run(thread2_func, &count, "test_tread");
	if (IS_ERR(kthread_2)) {
		printk(KERN_EMERG "Unable to start kernel thread 2.\n");
		return 0;
	}
	wait_for_completion(&complete_th1);
	wait_for_completion(&complete_th2);
	printk(KERN_EMERG "count: %i\n", count);
	return 0;
}

static void __exit sync_exit(void)
{

}


module_init(sync_init);
module_exit(sync_exit);
