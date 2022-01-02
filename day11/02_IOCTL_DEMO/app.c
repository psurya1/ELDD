#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "command.h"

int main()
{
    int fd,val_1=20,val_2=40;
    char kbuff[40];
    char ubuff[40]="HEY DUDE..\n";
    fd=open("/dev/GREAT",O_RDWR,0777);
    if(fd<0)
    {
        printf("\n ERROR IN OPENING..!\n");
        exit(1);
    }
    ioctl(fd,SET_BAUD,&val_1);
    ioctl(fd,SET_STOP_BIT,&val_2);
    read(fd,kbuff,sizeof(kbuff));
    printf("RAED FUNCTION..%s\n",kbuff);
    close(fd);
}
