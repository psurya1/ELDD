// ioctl simple program demo

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/types.h>
#include<linux/uaccess.h>
#include<linux/ioctl.h>
#include<linux/cdev.h>
#include "command.h"

// declaration:
dev_t dev;
struct cdev *my_cdev;
int val_1,val_2;

//Protocol
int GREAT_open(struct inode *inode,struct file *filp);
ssize_t GREAT_read(struct file *filp,char __user *ubuff,size_t count,loff_t *offp);
ssize_t GREAT_write(struct file *filp,const char __user *ubuff,size_t count,loff_t *offp);
int GREAT_release(struct inode *inode,struct file *filp);
long GREAT_ioctl(struct file *filp,unsigned int cmd,unsigned long arg);

struct file_operations fops=
{
    .owner          =   THIS_MODULE,
    .open           =   GREAT_open,
    .read           =   GREAT_read,
    .write          =   GREAT_write,
    .unlocked_ioctl =   GREAT_ioctl,
    .release        =   GREAT_release,
};

static int __init prog_init(void)
{
    if((alloc_chrdev_region(&dev,88,1,"GREAT"))<0)
    {
        printk("\n cannot create major number..!!!\n");
        return -1;
    }
    printk("\n MAJOR AND MINOR <%d>:<%d>\n",MAJOR(dev),MINOR(dev));
    // creating cdev

    my_cdev=cdev_alloc();
    my_cdev->ops=&fops;

    // adding major and cdev

    if((cdev_add(my_cdev,dev,1))<0)
    {
        printk("\n cannot add major number and cdev..!!!\n");
        unregister_chrdev_region(dev,1);
        return -1;
    }
    printk(KERN_ALERT "\n DRIVER LOADED..!!\n");
    return 0;

}

static void __exit prog_exit(void)
{
    unregister_chrdev_region(dev,1);
    cdev_del(my_cdev);
    printk(KERN_ALERT "\n DRIVER UNLOADED..!!\n");
}
module_init(prog_init);
module_exit(prog_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SRA");

int GREAT_open(struct inode *inode,struct file *filp)
{
    printk("\n OPEN CALL..!\n");
    return 0;
}
int GREAT_release(struct inode *inode,struct file *filp)
{
    printk("\n CLOSE CALL..!\n");
    return 0;
}
ssize_t GREAT_read(struct file *filp,char __user *ubuff,size_t count,loff_t *offp)
{
    ssize_t retval;
    unsigned long result;
    char kbuff[60]="HEY DUDE THIS IS KERNEL...\n";
    result=copy_to_user((char*)ubuff,(char*)kbuff,count);
    if(result==0)
    {
        printk(KERN_INFO "\n DATA TO USER....>> %s << \n",kbuff);
        printk(KERN_INFO "\n DATA SEND COMPLETLY..\n");
        retval=count;
        return retval;
    }
    else if(result>0)
    {
        printk(KERN_INFO "\n PART OF DATA SENDED..>> %s <<\n",kbuff);
        retval=(count-result);
        return retval;
    }
    else
    {
        printk(KERN_INFO "\n ERROR IN READING..\n");
        retval=-EFAULT;
        return retval;
    }
}
ssize_t GREAT_write(struct file *filp,const char __user *ubuff,size_t count,loff_t *offp)
{
    ssize_t retval;
    unsigned long result;
    char kbuff[60];
    result=copy_from_user((char*)kbuff,(char*)ubuff,count);
    if(result==0)
    {
        printk(KERN_INFO "\n DATA FROM USER....>> %s << \n",ubuff);
        printk(KERN_INFO "\n DATA RECEIVED COMPLETLY..\n");
        retval=count;
        return retval;
    }
    else if(result>0)
    {
        printk(KERN_INFO "\n PART OF DATA RECEIVED..>> %s <<\n",ubuff);
        retval=(count-result);
        return retval;
    }
    else
    {
        printk(KERN_INFO "\n ERROR IN WRITING..\n");
        retval=-EFAULT;
        return retval;
    }
}
long GREAT_ioctl(struct file *filp,unsigned int cmd,unsigned long arg)
{
    switch(cmd)
    {
        case SET_BAUD:
            copy_from_user(&val_1,(int *)arg,sizeof(val_1));
            printk("\n VALUE 1 ==%d\n",val_1);
            break;
        case SET_STOP_BIT:
            copy_from_user(&val_2,(int *)arg,sizeof(val_2));
            printk("\n VALUE 2 ==%d\n",val_2);
            break;
    }
    return 0;
}

