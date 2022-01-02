// writer 1

#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main()
{
    int fd;
    ubuff[40]="THIS IS FROM USER TO KERNEL.\n";
    fd=open("/dev/AUS",O_RDWR,0777);
    if(fd<0)
    {
        printf("ERROR IN OPENING.\n");
        exit(1);
    }
    write(fd,ubuff,sizeof(ubuff));
    close(fd);
}