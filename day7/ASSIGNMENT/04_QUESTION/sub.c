#include<stdio.h>                          //second application.......
#include<stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main()
{
    int fd;
    int ubuff[60]="THIS IS FROM SUBTRACTION..\n";
    
//  char ubuff[100]="THIS MESSAGE FROM USER..\n";  
    char b[100];
    
    fd=open("/dev/calculator",O_RDWR,0777);
    if(fd<0)
    {
        printf("ERROR IN OPENING\n");
        exit(1);
    }
    write(fd,ubuff,sizeof(ubuff));
    read(fd,b,sizeof(b));
    printf("OUPUT ...%s..\n",b);
   

    
    close(fd);
}
