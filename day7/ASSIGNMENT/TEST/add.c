#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main()
{
    int fd;
     
  int array[2]={10,20};  
  char a[100];
    
    fd=open("/dev/ADD",O_RDWR,0777);
    if(fd<0)
    {
        printf("ERROR IN OPENING\n");
        exit(1);
    }
    write(fd,array,sizeof(array));
    read(fd,a,sizeof(a));
    printf("OUPUT MUST 30...%s..\n",a);
   

    
    close(fd);
}
