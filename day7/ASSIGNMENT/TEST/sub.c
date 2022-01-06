#include<stdio.h>                          //second application.......
#include<stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main()
{
    int fd;
    int ubuff[60]={20,10};
     
    char b[100];
    
    fd=open("/dev/SUB",O_RDWR,0777);
    if(fd<0)
    {
        printf("ERROR IN OPENING\n");
        exit(1);
    }
    write(fd,ubuff,sizeof(ubuff));
    read(fd,b,sizeof(b));
    printf("OUPUT  MUST 10...%s..\n",b);
   

    
    close(fd);
}
