// sample program for interrupt handling...

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/types.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/interrupt.h>

#define IRQ_NO 2
unsigned int i=0;
//interrupt handler for IRQ 1.
static irqreturn_t irq_handler(int irq,void *dev_id)
{
    printk(KERN_INFO "\n interrupt occured %d\n",i);
    i++;
    return IRQ_HANDLED;
}
dev_t dev;
static struct cdev my_cdev;
//Function declaration:
int POW_open(struct inode *inode,struct file *filp);
ssize_t POW_read(struct file *filp,char __user *ubuff,size_t count,loff_t *offp);
ssize_t POW_write(struct file *filp, const char __user *ubuff,size_t count,loff_t *offp);
int POW_release(struct inode *inode,struct file *filp);

struct file_operations fops=
{
    .owner  =   THIS_MODULE,
    .open   =   POW_open,
    .read   =   POW_read,
    .write  =   POW_write,
    .release=   POW_release,
};

//definition:

int POW_open(struct inode *inode,struct file *filp)
{
    printk("\n OPEN FUNCTION..\n");
    return 0;
}
int POW_release(struct inode *inode,struct file *filp)
{
    printk("\n CLOSE FUNCTION..\n");
    return 0;
}
ssize_t POW_read(struct file *filp,char __user *ubuff,size_t count,loff_t *offp)
{
    printk("\n READ FUNCTION..\n");
    return 0;
}    
ssize_t POW_write(struct file *filp,const char __user *ubuff,size_t count,loff_t *offp)
{
    printk("\n WRITE FUNCTION..\n");
    return count;
}   
static int __init prog_init(void)
{
    if((alloc_chrdev_region(&dev,99,1,"POW"))<0)
    {
        printk("\n cannot create major number..\n");
        return -1;
    }
    printk("\n MAJOR <%d> MINOR <%d>",MAJOR(dev),MINOR(dev));
    // cdev structure

    cdev_init(&my_cdev,&fops);

    // addind cdev and major

    if((cdev_add(&my_cdev,dev,1))<0)
    {
        printk("\n cannot add cdev and major..\n");
        unregister_chrdev_region(dev,1);
        return -1;
    }
    if(request_irq(IRQ_NO,irq_handler,IRQF_SHARED,"POW",(void*)(irq_handler)))
    {
        printk(KERN_ALERT "\n cannot register the irq number.!!\n");
        free_irq(IRQ_NO,(void*)(irq_handler));
    }
    printk("\n DRIVER LOADED..\n");
    return 0;
}
static void __exit prog_exit(void)
{
    free_irq(IRQ_NO,(void*)(irq_handler));
    unregister_chrdev_region(dev,1);
    cdev_del(&my_cdev);
    printk("\n DRIVER UNLOADED..\n");
}
module_init(prog_init);
module_exit(prog_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SRA");
