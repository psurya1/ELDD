// QUESTION 4:

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/device.h>

ssize_t read_add(struct file *filp,char __user *ubuff,size_t count,loff_t *offp);
ssize_t write_add(struct file *filp, const char __user *ubuff,size_t count,loff_t *offp);

ssize_t read_sub(struct file *filp,char __user *ubuff,size_t count,loff_t *offp);
ssize_t write_sub(struct file *filp, const char __user *ubuff,size_t count,loff_t *offp);

ssize_t read_mul(struct file *filp,char __user *ubuff,size_t count,loff_t *offp);
ssize_t write_mul(struct file *filp, const char __user *ubuff,size_t count,loff_t *offp);

ssize_t read_div(struct file *filp,char __user *ubuff,size_t count,loff_t *offp);
ssize_t write_div(struct file *filp, const char __user *ubuff,size_t count,loff_t *offp);


#define NO_OF_DEVICES 4
// DEVICE PRIVATE DATA
struct char_dev_private_data
{
    const char *name;
    struct cdev cdev;
};
// DRIVER PRIVATE DATA
struct char_drv_private_data
{
    int total_devices;
    struct device_number;
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

struct file_operations temp=
{
    .owner  =   THIS_MODULE,
    .open   =   default_open,
    .read   =   default_read,
    .write  =   default_write,
    .release=   default_release,
};

temp=filp->ops;

static int __init prog_init(void)
{
    int ret,i;
    // Dynamically allocate device number
    ret=alloc_chrdev_region(&char_drv_data.device_number,0,NO_OF_DEVICES,"calculator");
    if(ret<0)
    {
        printk(KERN_ALERT "\n cannot allocated major number");
        return -1;
    }
    for(i=0;i<NO_OF_DEVICES;i++)
    {
        printk(KERN_INFO "DEVICE NUMBER <major>:<minor>=%d:%d\n",MAJOR(char_drv_data.device_number+i),MINOR(char_drv_data.device_number+i));
        // initialize the cdev structure with fops
        cdev_init(&char_drv_data.char_dev_data[i].cdev,&temp);

        // add major and cdev
        ret=cdev_add(&char_drv_data.char_dev_data[i].cdev,char_drv_data.device_number+i,1);
        if(ret<0)
        {   
            printk(KERN_ALERT "\n CDEV ADD FAILED..\n");
            for(;i>0;i--)
            {
                cdev_del(&char_drv_data.char_dev_data[i].cdev);
            }
            
        }
    }
    printk(KERN_ALERT "\n MODULE INIT WAS SUCCESSFUL.\n");
    return 0;
}
MODULE_AUTHOR("GPL");
MODULE_AUTHOR("SRA");
static void __exit prog_exit(void)
{
    int i;
    for(i=0;i<NO_OF_DEVICES;i++)
    {
        cdev_del(&char_drv_data.char_dev_data[i].cdev);
    }
    unregister_chrdev_region(char_drv_data.device_number,NO_OF_DEVICES);
    printk(KERN_ALERT "\n MODULE UNLOADED.\n");
}

