//spin lock in process

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/types.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/uaccess.h>

dev_t dev;
struct cdev my_cdev;
struct class *dev_class;
spinlock_t lock;

//Function declaration:
int STR_open(struct inode *inode,struct file *filp);
ssize_t STR_read(struct file *filp,char __user *ubuff,size_t count,loff_t *offp);
ssize_t STR_write(struct file *filp, const char __user *ubuff,size_t count,loff_t *offp);
int STR_release(struct inode *inode,struct file *filp);

struct file_operations fops=
{
    .owner  =   THIS_MODULE,
    .open   =   STR_open,
    .read   =   STR_read,
    .write  =   STR_write,
    .release=   STR_release,
};

static int __init prog_init(void)
{
    if((alloc_chrdev_region(&dev,45,1,"STR"))<0)
    {
        printk("\n cannot allocated major number.\n");
        return -1;
    }
    printk("MAJOR %d :MINOR %d\n",MAJOR(dev),MINOR(dev));
    cdev_init(&my_cdev,&fops);
    if((cdev_add(&my_cdev,dev,1))<0)
    {
        printk("\n cannot add major and cdev.\n");
        unregister_chrdev_region(dev,1);
        return -1;
    }
    if((dev_class=class_create(THIS_MODULE,"STR_class"))==NULL)
    {
        printk("\n cannot create class.\n");
        unregister_chrdev_region(dev,1);
        return -1;
    }
    if((device_create(dev_class,NULL,dev,NULL,"STR_device"))==NULL)
    {
        printk("\n cannot create device.\n");
        class_destroy(dev_class);
    } 
    spin_lock_init(&lock);
    printk("\n DRIVER LOADED.!\n");
    return 0;
}
static void __exit prog_exit(void)
{
    device_destroy(dev_class,dev);
    class_destroy(dev_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev,1);
    printk(" DRIVER UNLOADED.!\n");  
}
module_init(prog_init);
module_exit(prog_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SRA");

int STR_open(struct inode *inode,struct file *filp)
{
    printk("\n OPEN FUNCTION..\n");
    return 0;
}
int STR_release(struct inode *inode,struct file *filp)
{
    printk("\n CLOSE FUNCTION..\n");
    return 0;
}
ssize_t STR_read(struct file *filp,char __user *ubuff,size_t count,loff_t *offp)
{
    unsigned long result;
    ssize_t retval;
    char kbuff[60]="HEY DUDE CHECKING..\n";
    spin_lock(&lock);
    result=copy_to_user((char *)ubuff,(char *)kbuff,sizeof(kbuff));
    spin_unlock(&lock);
    if(result==0)
    {
        printk("\n MESSAGE TO SEND..%s\n",kbuff);
        printk("\n DATA SEND COMPLETE.\n");
        retval=count;
        return retval;
    }
    else if(result>0)
    {
        printk("\n PART OF DATA SEND..%s\n",kbuff);
        retval=(count-result);
        return retval;
    }
    else
    {
        printk("\n ERROR IN READING.\n");
        retval=-EFAULT;
        return retval;
    }

}
ssize_t STR_write(struct file *filp, const char __user *ubuff,size_t count,loff_t *offp)
{
    unsigned long result;
    ssize_t retval;
    char kbuff[60];
    spin_lock(&lock);
    result=copy_from_user((char *)kbuff,(char *)ubuff,sizeof(ubuff));
    spin_unlock(&lock);
    if(result==0)
    {
        printk("\n MESSAGE FROM USER..%s\n",ubuff);
        printk("\n DATA RECEIVED COMPLETE.\n");
        retval=count;
        return retval;
    }
    else if(result>0)
    {
        printk("\n PART OF DATA RECEIVE..%s\n",ubuff);
        retval=(count-result);
        return retval;
    }
    else
    {
        printk("\n ERROR IN WRITING.\n");
        retval=-EFAULT;
        return retval;
    }


}

