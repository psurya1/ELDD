
// PRODUCER 1....................

#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc,const char *argv[])
{
    int fd;
    
    char ubuff[60]="THIS IS FROM USER SPACE BY PROCEDUR 1..\n";
    fd=open("/dev/mydevice20",O_RDWR,0777);
    if(fd<0)
    {
        printf("ERROR IN OPENING..\n");
        exit(1);
    }
    write(fd,ubuff,sizeof(ubuff));
    
    close(fd);
    return 0;
}
