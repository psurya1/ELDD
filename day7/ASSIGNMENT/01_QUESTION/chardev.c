/* 1.Write a character driver with open and close functionality
     Test the driver by writing an mapplicationthatopensandclosesthedevicedriver.
     Whenopenorclosed,theopenandclosecallsinthedrivershouldbeexecuted.  */

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/cdev.h>

#define NAME mydevice1

//declaring protocol
int NAME_open(struct inode *inode,struct file *filp);
int NAME_release(struct inode *inode,struct file *filp);

struct file_operations fops=
{
    .owner = THIS_MODULE,
    .open  = NAME_open,
    .release=NAME_release,
};

struct cdev *my_cdev;
static int __init prog_init(void)
{
    int result;
    int MAJOR,MINOR;
    dev_t MYDEV;
    MYDEV=MKDEV(3,0);
    MAJOR=MAJOR(MYDEV);
    MINOR=MINOR(MYDEV);
    printk(KERN_INFO "\n THE MAJOR NUMBER %d..THE MINOR NUMBER %d..\n",MAJOR,MINOR);
    result=register_chrdev_region(MYDEV,1,"mydevice1");
    if(result<0)
    {
        printk(KERN_ALERT "\n THE REQUESTED REGION NOT OBTANIED..\n");
        return(-1);    
    }
    my_cdev=cdev_alloc();
    my_cdev->ops=&fops;
    result=cdev_add(my_cdev,MYDEV,1);
    if(result<0)
    {
        printk(KERN_ALERT "\n WE CANNOT REGISTER DEVICE NUMBER AND CDEV..\n");
        cdev_del(my_cdev);
        return(-1);
    }
    return 0;
}
static void __exit prog_exit(void)
{   
    int result;
    int MAJOR,MINOR;
    dev_t MYDEV;
    MYDEV=MKDEV(3,0);
    MAJOR=MAJOR(MYDEV);
    MINOR=MINOR(MYDEV);
    printk(KERN_INFO "\n THE MAJOR NUMBER %d..THE MINOR NUMBER %d..\n",MAJOR,MINOR);
    unregister_chrdev_region(3,0);
    cdev_del(my_cdev);
    printk(KERN_INFO "\n I HAVE CLEAR ALL STEPS FROM INIT..\n");
     
}
module_init(prog_init);
module_exit(prog_exit);
//definition of function
int NAME_open(struct inode *inode,struct file *filp)
{
    printk(KERN_INFO "\n THIS IS OPEN_SYSTEM CALL");
    return 0;
}
int NAME_release(struct inode *inode,struct file *filp)
{
    printk(KERN_INFO "\n THIS IS CLOSE_SYSTEM CALL");
    return 0;
}
