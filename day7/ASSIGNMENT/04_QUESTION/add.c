#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main()
{
    int fd;
     
  char ubuff[60]="THIS MESSAGE FROM ADDITION..\n";  
    char a[100];
    
    fd=open("/dev/calculator",O_RDWR,0777);
    if(fd<0)
    {
        printf("ERROR IN OPENING\n");
        exit(1);
    }
    write(fd,ubuff,sizeof(ubuff));
    read(fd,a,sizeof(a));
    printf("OUPUT ...%s..\n",a);
   

    
    close(fd);
}
