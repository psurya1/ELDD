#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include "add.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SRA");

static int one=5;
static int two=5;

static int __init prog_init(void)
{
        printk(KERN_ALERT "\n HI DUDE...\n");
	printk(KERN_ALERT "\n THE VALUE ... %d\n",add(one,two));
        return 0;
}
static void __exit prog_exit(void)
{
        printk(KERN_ALERT "\n BYE DUDE..\n");
}

module_init(prog_init);
module_exit(prog_exit);
