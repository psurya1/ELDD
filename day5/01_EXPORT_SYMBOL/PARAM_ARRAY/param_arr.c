#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SRA");

static int myarray[2]={0};
static int count=1;

module_param_array(myarray,int,&count,S_IRUGO);

static int __init prog_init(void)
{
    printk(KERN_ALERT "HI DUDE\n");
    size_t i=0;
    while(i<count)
    {
        printk("\n MYARRAY %d\n",myarray[i]);
        i++;
    }   
    return 0;
}
static void __exit prog_exit(void)
{
        printk(KERN_ALERT "BYE DUDE\n");
}

module_init(prog_init);
module_exit(prog_exit);
