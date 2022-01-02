#include<stdio.h>                          //second application.......
#include<stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main()
{
    int fd;
    int ubuff2[60]="THIS IS FROM MULTIPLICATION..\n";
    
//  char ubuff1[100]="THIS MESSAGE FROM USER..\n";  
    char c[100];
    
    fd=open("/dev/calculator",O_RDWR,0777);
    if(fd<0)
    {
        printf("ERROR IN OPENING\n");
        exit(1);
    }
    write(fd,ubuff2,sizeof(ubuff2));
    read(fd,c,sizeof(c));
    printf("OUPUT ...%s..\n",c);
   

    
    close(fd);
}
