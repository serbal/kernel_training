#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>

MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");


extern int print_hello(void)
{
	printk(KERN_EMERG "Hello, world!\n");
	return 0;
}
EXPORT_SYMBOL(print_hello);

static int __exit hello_init(void)
{
	return 0;
}

static void __exit hello_exit(void)
{

}


module_init(hello_init);
module_exit(hello_exit);
