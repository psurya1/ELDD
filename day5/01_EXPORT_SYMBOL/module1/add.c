#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SRA");

static int add(int a,int b)
{
	return(a+b);
}

EXPORT_SYMBOL(add);

static int __init prog_init(void)
{
	printk(KERN_ALERT "\n HI DUDE...\n");
	return 0;
}
static void __exit prog_exit(void)
{
	printk(KERN_ALERT "\n BYE DUDE..\n");
}

module_init(prog_init);
module_exit(prog_exit);
