//wait queue program demo

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/wait.h>
#include<linux/kthread.h>



//struct class *dev_class;
struct cdev my_cdev;
dev_t dev;
wait_queue_head_t wq;
int wait_flag=0;

//declaration:
int RSA_open(struct inode *inode,struct file *filp);
ssize_t RSA_read(struct file *filp,char __user *ubuff,size_t count,loff_t *offp);
ssize_t RSA_write(struct file *filp, const char __user *ubuff,size_t count,loff_t *offp);
int RSA_release(struct inode *inode,struct file *filp);

struct file_operations fops=
{
    .owner  =   THIS_MODULE,
    .open   =   RSA_open,
    .read   =   RSA_read,
    .write  =   RSA_write,
    .release=   RSA_release,
};




static int __init RSA_init(void)
{
    
    if((alloc_chrdev_region(&dev,25,1,"RSA"))<0)
    {
        printk("\n cannot create major number..");
        return -1;
    }
    printk(KERN_ALERT "\n major number =%d,minor number =%d",MAJOR(dev),MINOR(dev));

    // creating cdev structure
    cdev_init(&my_cdev,&fops);

    // adding major number with cdev

    if((cdev_add(&my_cdev,dev,1))<0)
    {
        printk("\n cannot add major number and cdev.\n");
        goto r_class;
    }

   /* // creating struct class
    if((dev_class=class_create(THIS_MODULE,"RSA_class"))==NULL)
    {
        printk("\n cannot create class.\n");
        goto r_class;
    }
    // creating struct device
    if((device_create(dev_class,NULL,dev,NULL,"RSA_device"))==NULL)
    {
        printk("\n cannot create device.\n");
        goto r_device;
    }
    

r_device:
        class_destroy(dev_class);
        */
r_class:
        unregister_chrdev_region(dev,1);
        return -1; 

    // initialize wait queue
    init_waitqueue_head(&wq);

   
    printk("\n device driver loaded..\n");
    return 0;
}

static void __exit RSA_exit(void)
{
    wait_flag=2;
    wake_up_interruptible(&wq);
 //   class_destroy(dev_class);
 //   device_destroy(dev_class,dev);
    unregister_chrdev_region(dev,1);
    cdev_del(&my_cdev);
    printk("device driver unloaded..\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SRA");
module_init(RSA_init);
module_exit(RSA_exit);


int RSA_open(struct inode *inode,struct file *filp)
{
    printk("\n device file opened..\n");
    return 0;
}
ssize_t RSA_read(struct file *filp,char __user *ubuff,size_t count,loff_t *offp)
{

    char kbuff[60]="THIS IS MESSAGE FROM KERNEL....";
    unsigned long result;
    ssize_t retval;
    result=copy_to_user((char*)ubuff,(char*)kbuff,count);
    if(result==0)
    {
        printk(KERN_ALERT "\n MESSAGE TO USER..\n...%s....\n",kbuff);
        printk(KERN_INFO  "\n DATA SEND COMPLETED..\n");
        retval=count;
        return retval;
    }
    else if(result>0)
    {  
        printk(KERN_ALERT "\n MESSAGE TO USER..\n...%s....\n",kbuff);
        printk(KERN_ALERT "\n THE PART OF DATA IS SENDED..\n ");
        retval=(count-result);
        return retval;
    }
    else
    {
        printk(KERN_ALERT "\n ERROR IN READING");
        retval=-EFAULT;
        return retval;
    }
    wait_flag=1;
    wake_up_interruptible(&wq);
    
}
ssize_t RSA_write(struct file *filp, const char __user *ubuff,size_t count,loff_t *offp)
{
    while(1)
    {
        printk("\n waiting for event ..\n");
        wait_event_interruptible(wq,wait_flag!=0);
        if(wait_flag==2)
        {
            printk("event came from exit function..!\n");
            return 0;
        }

        char kbuff[60];
        unsigned long result;
        ssize_t retval;
        result=copy_from_user((char*)kbuff,(char*)ubuff,count);
        if(result==0)
        {
            printk(KERN_ALERT "\n MESSAGE FROM USER..\n...%s....\n",ubuff);
            printk(KERN_INFO  "\n DATA RECEIVED COMPLETED..\n");
            retval=count;
            return retval;
        }
        else if(result>0)
        {  
            printk(KERN_ALERT "\n MESSAGE FROM USER..\n...%s....\n",ubuff);
            printk(KERN_ALERT "\n THE PART OF DATA IS RECEIVED..\n ");
            retval=(count-result);
            return retval;
        }
        else
        {
            printk(KERN_ALERT "\n ERROR IN WRITING");
            retval=-EFAULT;
            return retval;
        }
    }
}
int RSA_release(struct inode *inode,struct file *filp)
{
    printk("\n device file opened..\n");
    return 0;
}