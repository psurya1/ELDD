// character driver w1 write r1 should read should not mismatch 


#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/semaphore.h>


dev_t dev;
static struct cdev my_cdev;
struct semaphore sem_1;
struct semaphore sem_2;

//protocol
int NAME_open(struct inode *inode,struct file *filp);
ssize_t NAME_read(struct file *filp,char __user *ubuff,size_t count,loff_t *offp);
ssize_t NAME_write(struct file *filp, const char __user *ubuff,size_t count,loff_t *offp);
int NAME_release(struct inode *inode,struct file *filp);

struct file_operations fops=
{
    .owner =THIS_MODULE,
    .open  =NAME_open,
    .read  =NAME_read,
    .write =NAME_write,
    .release=NAME_release,
};
static int __init prog_init(void)
{
    int result;
    int MAJOR,MINOR;
    MYDEV=MKDEV(35,0);
    MAJOR=MAJOR(MYDEV);
    MINOR=MINOR(MYDEV);
    printk(KERN_INFO "\n THE MAJOR NUMBER %d.. THE MINOR NUMBER %d..\n",MAJOR,MINOR);
    result=register_chrdev_region(MYDEV,1,"SEM");
    if(result<0)
    {
        printk(KERN_INFO "\n THE DEVICE NUMBER IS NOT REGISTERED..\n");
        return(-1);
    }
    my_cdev=cdev_alloc();
    my_cdev->ops=&fops;

    result=cdev_add(my_cdev,MYDEV,1);
    if(result<0)
    {
        printk(KERN_INFO "\n THE DEVICE NUMBER AND CDEV NOT CONNECTED.\n");
        unregister_chrdev_region(MYDEV,1);
        return(-1);
    }

    // semaphore initialize
    sema_init(&sem_1,1);
    sema_init(&sem_2,1);
    /* completion initialize
    init_completion(&data_read_done);
    init_completion(&data_write_done); */
    
    printk(KERN_ALERT "\n SEMAPHORE IS INITAILIZED.. AND DRIVER LOADED..\n");
    return 0;
    
}
static void __exit prog_exit(void)
{
    int MAJOR,MINOR;
    MYDEV=MKDEV(35,0);
    MAJOR=MAJOR(MYDEV);
    MINOR=MINOR(MYDEV);
    printk(KERN_INFO "\n THE MAJOR NUMBER %d.. THE MINOR NUMBER %d..\n",MAJOR,MINOR);
    unregister_chrdev_region(MYDEV,1);
    cdev_del(my_cdev);
    printk(KERN_INFO "\n I HAVE REMOVED ALL THE INIT....\n");
}
module_init(prog_init);
module_exit(prog_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SRA");
//global declaration:

kbuff[60];

int SEM_open(struct inode *inode,struct file *filp)
{
    
    printk(KERN_ALERT "\n THE OPEN SYSTEM CALL IS CALLED...\n");
    return 0;
}
int SEM_release(struct inode *inode,struct file *filp)
{
    
    printk(KERN_ALERT "\n THE OPEN SYSTEM CALL IS CALLED...\n");
    return 0;
}

ssize_t SEM_read(struct file *filp,char *ubuff,size_t count,loff_t *off)
{
    ssize_t retval=-1;
    unsigned long res;
    res=copy_to_user((char *)ubuff,(char *)kbuff,count);
    if(res==0)
    {   
        retval=count;
        return retval;
    }
    else if(res>0)
    {
        retval=count-result;
        return retval;
    }
    printk("ERROE IN READING..\n");
    return retval;
}

ssize_t SEM_write(struct file *filp,char *ubuff,size_t count,loff_t *off)
{
    ssize_t retval=-1;
    unsigned long res;
    res=copy_from_user((char*)kbuff,(char *)ubuff,count);
    if(res==0)
    {   
        retval=count;
        return retval;
    }
    else if(res>0)
    {
        retval=count-result;
        return retval;
    }
    printk("ERROE IN WRITING..\n");
    return retval;
}
