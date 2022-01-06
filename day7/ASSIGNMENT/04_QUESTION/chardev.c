/* •Write a calculator driver in the kernel which performs the following
   •Create 4 Device Numbers –Each device number depicts a specific calculation operation 
    likeadd,subtract,multiply and divide 
   •Implement 8 methods–Read_Add,Read_Sub,Read_Mul,Read_Div,Write_Add,Write_Sub,Write_MulandWrite_Div
   •Inuserspace,create 4 device nodes for the 4 device numbers created–/dev/AddDev,/dev/SubDev,/dev/MulDev,/dev/DivDev
   •Write 4 user applications in the user space to test the above.
    If/dev/AddDev application is run,it should write 2 numbers to the kernel and
    the kernel should add it and return the kbuff in the subsequentread.  */

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
int sum;
ssize_t char_dev_read(struct file *filp,char __user *ubuff,size_t count,loff_t *f_pos)
{
    char kbuff[40];
    unsigned long result;
    ssize_t retval;
    sprintf(kbuff,"%d",sum);
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
}


ssize_t char_dev_write(struct file *filp,const char __user *ubuff,size_t count,loff_t *f_pos)
{
    int kbuff[40];
    unsigned long result;
    ssize_t retval;
    result=copy_from_user((char*)kbuff,ubuff,count);
    sum=(int)kbuff[0]+(int)kbuff[1];
    if(result==0)
    {
        printk(KERN_INFO "\n MESSAGE FROM USER..\n...%d....\n",(int)kbuff[0],(int)kbuff[1]);
        printk(KERN_INFO  "\n DATA RECEIVED COMPLETED..\n");
        retval=count;
        return retval;
    }
    else if(result>0)
    {  
   //   printk(KERN_ALERT "\n MESSAGE FROM USER..\n...%s....\n",ubuff);
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

//FILE OPERATION
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
        unregister_chrdev_region(char_drv_data.device_number,NO_OF_DEVICES);
 
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
            for(;i>0;i--)
        {
            device_destroy(char_drv_data.class_char_dev,char_drv_data.device_number+i);
            cdev_del(&char_drv_data.char_dev_data[i].cdev);
        }
        class_destroy(char_drv_data.class_char_dev);
        }
        // populate the sysfs with device number
        char_drv_data.device_char_dev=device_create(char_drv_data.class_char_dev,NULL,char_drv_data.device_number+i,NULL,"dev-%d",i+1);
        if(IS_ERR(char_drv_data.device_char_dev))
        {
            printk(KERN_ALERT "\n DEVICE CREATE FAILED..\n");
            ret=PTR_ERR(char_drv_data.device_char_dev);
            for(;i>0;i--)
        {
            device_destroy(char_drv_data.class_char_dev,char_drv_data.device_number+i);
            cdev_del(&char_drv_data.char_dev_data[i].cdev);
        }
        class_destroy(char_drv_data.class_char_dev);
        }
    }     
    printk(KERN_ALERT "\n MODULE INIT WAS SUCCESSFUL.\n");
    return 0;
}
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SRA");
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

