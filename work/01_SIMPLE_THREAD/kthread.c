/* sample program of thtread  */

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kthread.h>
#include<linux/sched.h>
#include<linux/delay.h>

/* Global variables for the threads */
static struct task_struct *kthread_1;
static struct task_struct *kthread_2;
static int t1 = 1,t2 = 2;

int thread_function(void * thread_no)
{
    unsigned int i=0;
    int t_no =*(int*)thread_no;

    // working loop
    while(!kthread_should_stop())
    {
        printk("kthread -Thread %d finished execution %d\n",t_no,i++);
        msleep(t_no *1000);
    }
    printk("kthread -Thread %d finished execution\n",t_no);
    return 0;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SRA");

static int __init prog_init(void)
{
    printk("\n Kthread -init thread..\n");
    kthread_1 = kthread_run(thread_function,&t1,"kthread_1");
    if(kthread_1 !=NULL)
    {
        printk("\n kthread - Thread 1 was created and running..\n");
    }
    else
    {
        printk("\n kthread - Thread 1 was not  created and running..\n");
        kthread_stop(kthread_1);
        return -1;
    }
    kthread_2 = kthread_run(thread_function,&t2,"kthread_2");
    if(kthread_2 !=NULL)
    {
        printk("\n kthread - Thread 2 was created and running..\n");
    }
    else
    {
        printk("\n kthread - Thread 2 was not  created and running..\n");
        kthread_stop(kthread_2);
        return -1;
    }
    printk("\n kthread - Both threads are running now..\n ");
    return 0;
}

static void __exit prog_exit(void)
{
    printk("\n kthread - stop both threads..\n");
    kthread_stop(kthread_1);
    kthread_stop(kthread_2);
}

module_init(prog_init);
module_exit(prog_exit);