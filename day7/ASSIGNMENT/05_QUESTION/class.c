/* character driver for automatically create "mknod"...*/

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/types.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/device.h>
#include<linux/slab.h>


struct cdev my_cdev;
struct class *dev_class;
dev_t dev;

//declaration :
int SRA_open(struct inode *inode,struct file *filp);
ssize_t SRA_read(struct file *filp,char __user *ubuff,size_t count,loff_t *offp);
ssize_t SRA_write(struct file *filp, const char __user *ubuff,size_t count,loff_t *offp);
int SRA_release(struct inode *inode,struct file *filp);



struct file_operations fops=
{
    .owner  =   THIS_MODULE,
    .open   =   SRA_open,
    .read   =   SRA_read,
    .write  =   SRA_write,
    .release=   SRA_release,
};




static int __init prog_init(void)
{
    
    if(alloc_chrdev_region(&dev,20,1,"SRA")<0)
    {
        printk(KERN_ALERT "\n cannot allocate major number....\n");
        return(-1);
    }

    printk(KERN_INFO "\n THE MAJOR MINOR NUMBER <%d>:<%d> \n",MAJOR(dev),MINOR(dev));

    // adding device number with cdev

   cdev_init(&my_cdev,&fops);
    
    // adding charcter driver to the system  

    if(cdev_add(&my_cdev,dev,1)<0)
    {
        printk(KERN_ALERT "\n cannot register cdev and major number....\n");
        unregister_chrdev_region(dev,1);
        return(-1);
    }

    // create struct class

    if((dev_class=class_create(THIS_MODULE,"SRA_class"))==NULL)
    {
        printk(KERN_ALERT "\n cannot create the struct class..\n");
        unregister_chrdev_region(dev,1);
        return(-1);
    }

    // create device 

    if((device_create(dev_class,NULL,dev,NULL,"SRA_device"))==NULL)
    {
        printk(KERN_ALERT "\n cannot create the device..\n");
        class_destroy(dev_class);
    } 
    
    printk(KERN_INFO "\n DRIVER LOADED..\n");
    return 0;
        
}


static void __exit prog_exit(void)
{
    
    
    device_destroy(dev_class,dev);
    class_destroy(dev_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev,1);
    printk(KERN_INFO "\n DRIVER UNLOADED..\n");
}
module_init(prog_init);
module_exit(prog_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SRA");

int SRA_open(struct inode *inode,struct file *filp)
{
    printk(KERN_INFO "\n OPEN CALL..\n");
    return 0;
}
int SRA_release(struct inode *inode,struct file *filp)
{
    printk(KERN_INFO "\n CLOSE CALL..\n");
    return 0;
}
ssize_t SRA_read(struct file *filp ,char __user *ubuff,size_t count,loff_t *offp)
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
ssize_t SRA_write(struct file *filp ,const char __user *ubuff,size_t count,loff_t *offp)
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