#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



int main(int argc, char * argv[])
{
        
        int fd = open("/dev/mychardev-0", O_RDWR);
                
        if(fd == -1)
        {
            perror("open () failed ");
            exit(1);
        }
        
        char buf[20];
        
        memset(buf,0, 20);
        
        ssize_t ret=  read(fd, &buf[0], 10);
        
        
        if(ret == -1)
        {
            perror("read () failed ");
            exit(1);
        }
        
        
        printf("%s \n", buf);
        
        memset(buf,0, 20);
        
	for(int i=0;i<10;i++)
	{
		buf[i] = 'P';
	}	
        
        ssize_t ret1 =  write(fd, buf, 20);
        
        if(ret == -1)
        {
            perror("write () failed ");
            exit(1);
        
        }
        
        
        printf("%s \n",buf );

        int re2 = close(fd);
        
        if(re2 == -1)
        {
            perror("close () failed ");
            exit(1);
        
        }


    return 0;
}
