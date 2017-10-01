#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>

MODULE_AUTHOR("Sergii Balanets");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static int n = 1;
module_param(n, uint, 0644);

extern void print_hello(void);

static int __init init(void)
{
	WARN_ON(n == 0);
	BUG_ON(n > 10);
	if (n == 5)
		return -EINVAL;
	if (n == 2)
		try_module_get(THIS_MODULE);
	int i = 0;

	for (; i < n; i++)
		print_hello();
	return 0;
}

static void __exit print_goodbuy(void)
{
	u8 *dvar = (u8 *) __kmalloc(1, GFP_KERNEL);

	if (n == 3)
		*dvar = 0x90;
	printk(KERN_EMERG "Good bye cruel world\n");
}


module_init(init);
module_exit(print_goodbuy);
