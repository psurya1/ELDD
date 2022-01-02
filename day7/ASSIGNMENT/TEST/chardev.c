#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/device.h>

#define NO_OF_DEVICES 4

// DEVICE PRIVATE DATA STRUCTURE
struct char_dev_private_data
{
    const char *name;
    struct cdev cdev;
};
// DRIVERS PRIVATE DATA STRUCTURE 
struct char_drv_private_data
{
    int total_devices;
    dev_t device_number;

    struct class *class_char_dev;
    struct device *device_char_dev;
    struct char_dev_private_data char_dev_data[NO_OF_DEVICES];
};

struct char_drv_private_data char_drv_data=
{
    .total_devices=NO_OF_DEVICES,
    .char_dev_data={
        [0]={
            .name="ADD"
        },
        [1]={
            .name="SUB"
        },
        [2]={
            .name="MUL"
        },
        [3]={
            .name="DIV"
        }
    }
};
int char_dev_open(struct inode *inode,struct file *filp)
{
    int minor;
    struct char_dev_private_data *char_dev_data;
    /* find out on which device file open was attempted by the user space */
    minor=MINOR(inode->i_rdev);
    printk(KERN_INFO "\n MINOR ACCESS=%d\n",minor);

    /* get device's private data structure */
    char_dev_data=container_of(inode->i_cdev,struct char_dev_private_data,cdev);
    
    /* to supply device private data to other methods of the driver */
    filp->private_data=char_dev_data;

    printk(KERN_INFO "\n OPEN WAS SUCCESSFULL..\n");
    return 0;
}

int char_dev_release(struct inode *inode,struct file *filp)
{
    printk(KERN_INFO "\n RELEASE WAS SUCCESSFUL..\n");
    return 0;
}
ssize_t char_dev_read(struct file *filp,char __user *ubuff,size_t count,loff_t *f_pos)
{
    return 0;
}
ssize_t char_dev_write(struct file *filp,const char __user *ubuff,size_t count,loff_t *f_pos)
{
    return 0;
}

struct file_operations char_dev_fops=
{
    .owner  =THIS_MODULE, 
    .open   =char_dev_open,
    .read   =char_dev_read,
    .write  =char_dev_write,
    .release=char_dev_release,
};

    
    
    
static int __init prog_init(void)
{
    int ret;
    int i;
    /*Dynamically allocate device number */
    ret =alloc_chrdev_region(&char_drv_data.device_number,0,NO_OF_DEVICES,"calculator");
    if(ret<0)
    {
        printk(KERN_ALERT "\n ALLOC CHRDEV FAILED..\n");
        return(-1);
    }
    /*create device class under /sys/class/ */
    char_drv_data.class_char_dev=class_create(THIS_MODULE,"calculator");
    if(IS_ERR(char_drv_data.class_char_dev))
    {
        printk(KERN_ALERT "\n CLASS CREATION FAILED.\n");
        goto unreg_chrdev; 
    }

    for(i=0;i<NO_OF_DEVICES;i++)
    {
        printk(KERN_INFO "DEVICE NUMBER <major>:<minor>=%d:%d\n",MAJOR(char_drv_data.device_number+i),MINOR(char_drv_data.device_number+i));

        // initialize the cdev structure with fops
        cdev_init(&char_drv_data.char_dev_data[i].cdev,&char_dev_fops);

        // Register a device (cdev structure) with VFS
        char_drv_data.char_dev_data[i].cdev.owner=THIS_MODULE;
        ret=cdev_add(&char_drv_data.char_dev_data[i].cdev,char_drv_data.device_number+i,1);
        if(ret<0)
        {
            printk(KERN_ALERT "\n CDEV ADD FAILED..\n");
            goto cdev_del;
        }
        // populate the sysfs with device number
        char_drv_data.device_char_dev=device_create(char_drv_data.class_char_dev,NULL,char_drv_data.device_number+i,NULL,"dev-%d",i+1);
        if(IS_ERR(char_drv_data.device_char_dev))
        {
            printk(KERN_ALERT "\n DEVICE CREATE FAILED..\n");
            ret=PTR_ERR(char_drv_data.device_char_dev);
            goto class_del;
        }
    }     
    printk(KERN_ALERT "\n MODULE INIT WAS SUCCESSFUL.\n");
    return 0;

    cdev_del:
    class_del:
        for(;i>0;i--)
        {
            device_destroy(char_drv_data.class_char_dev,char_drv_data.device_number+i);
            cdev_del(&char_drv_data.char_dev_data[i].cdev);
        }
        class_destroy(char_drv_data.class_char_dev);
    unreg_chrdev:
        unregister_chrdev_region(char_drv_data.device_number,NO_OF_DEVICES);

    out:
        return ret;
}
// MODULE_LICENSE("GPL");
// MODULE_AUTHOR("SRA");
static void __exit prog_exit(void)
{
    int i;
    for(i=0;i<NO_OF_DEVICES;i++)
    {
        device_destroy(char_drv_data.class_char_dev,char_drv_data.device_number+i);
        cdev_del(&char_drv_data.char_dev_data[i].cdev);
    }
    class_destroy(char_drv_data.class_char_dev);
    unregister_chrdev_region(char_drv_data.device_number,NO_OF_DEVICES);
    printk(KERN_ALERT "\n MODULE UNLOADED.\n");
}

module_init(prog_init);
module_exit(prog_exit);
