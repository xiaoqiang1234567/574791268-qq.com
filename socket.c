#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<stdio.h>
#include<unistd.h>


int main ()
{
    int fd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in  addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9988);
    addr.sin_addr.s_addr = INADDR_ANY;

     int ret= bind(fd,(struct sockaddr *)&addr,sizeof(addr) );
     if(ret< 0)
     {
       perror("bind");
       return 0 ;
     }

    listen(fd , 250);
     int newfd =accept(fd,NULL,NULL);
    char buf [1024];

    read(newfd,buf ,sizeof(buf));
       printf("%s",buf);



}
