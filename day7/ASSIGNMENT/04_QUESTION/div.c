#include<stdio.h>                          //second application.......
#include<stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main()
{
    int fd;
    int ubuff3[60]="THIS IS FROM DIVISION..\n";
    
//  char ubuff1[100]="THIS MESSAGE FROM USER..\n";  
    char d[100];
    
    fd=open("/dev/calculator",O_RDWR,0777);
    if(fd<0)
    {
        printf("ERROR IN OPENING\n");
        exit(1);
    }
    write(fd,ubuff3,sizeof(ubuff3));
    read(fd,d,sizeof(d));
    printf("OUPUT ...%s..\n",d);
   

    
    close(fd);
}
