// demo program semaphore used between threads

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/types.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/semaphore.h>


dev_t dev;
struct cdev my_cdev;
static int global_var;
struct task_thread *kthread1;
struct task_thread *kthread2;


int thread_function1(void * arg);
int thread_function2(void * arg);


int thread_function1(void * arg)  //
{
    int result;
    result=copy_from_user((int *)global_var,(char *)ubuff,count);


    
}
//protocol
int GOOD_open(struct inode *inode,struct file *filp);
ssize_t GOOD_read(struct file *filp,char __user *ubuff,size_t count,loff_t *offp);
ssize_t GOOD_write(struct file *filp, const char __user *ubuff,size_t count,loff_t *offp);
int GOOD_release(struct inode *inode,struct file *filp);

struct file_operations fops=
{
    .owner =THIS_MODULE,
    .open  =GOOD_open,
    .read  =GOOD_read,
    .write =GOOD_write,
    .release=GOOD_release,
};

static int __init prog_init(void)
{
    if((alloc_chrdev_region(&dev,30,1,"GOOD"))<0)
    {
        printk("\n cannot create major number..\n");
        return -1;
    }
    // cdev structure

    cdev_init(&my_cdev,&fops);

    // adding major and cdev

    if((cdev_add(&my_cdev,dev,1))<0)
    {
        printk("\n cannot add major number cdev..\n");
        unregister_chrdev_region(dev,1);
        return -1
    }
    // create thread
    kthread1=kthread_run(thread_function1,NULL,"kthread_1");
    if(kthread1==NULL)
    {
        printk("\n THREAD 1 NOT CREATED..\n");
        kthread_stop(kthread1);
        return -1;
    }
    kthread2=kthread_run(thread_function2,NULL,"kthread_2");
    if(kthread2==NULL)
    {
        printk("\n THREAD 2 NOT CREATED..\n");
        kthread_stop(kthread2);
        return -1;
    }
    
    sema_init(sem,1);
    printk("\n SEMAPHORE INITAILIZED AND DRIVER LOADED..\n");
    return 0;
}
static void __exit prog_exit(void)
{
    unregister_chrdev_region(dev,1);
    cdev_del(&my_cdev);
    printk("\n DRIVER UNLOADED..\n");
}