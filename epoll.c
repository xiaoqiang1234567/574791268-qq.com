#include<stdio.h>
#include<sys/epoll.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<errno.h>
#include<stdlib.h>

void   addepoll(int epollfd,int fd)
{
    struct epoll_event ev;
    ev.data.fd=fd;
    ev.events=EPOLLIN;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);

}
int main ()
{
       int fd = socket(AF_INET,SOCK_STREAM,0);
        struct  sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port =htons(9987);
        addr.sin_addr.s_addr= INADDR_ANY;
        int ret =bind(fd,(struct sockaddr *)&addr,sizeof(addr));
        if(ret<0)
        {
            perror("bind");
            return 0 ;
        }
        listen(fd,250);
        
        int epollfd= epoll_create(1024);
        addepoll(epollfd,fd);
    struct epoll_event ev[8];
    while(1)
    {
        int ret= epoll_wait(epollfd,ev,8,5000);
        if(ret<0&&errno==EINTR||ret == 0)
        {
            continue;

        }
        else if(ret< 0)
        {
            perror("epoll_wait");
            exit(1);
        }
        int i;
        for( i = 0 ; i < ret ; i++)
        {
            int evfd = ev[i].data.fd;
            if(evfd == fd)
            {
                int newfd = accept(fd,NULL,NULL);
                addepoll(epollfd,newfd);
            }
            else
            {
                char buf[1024];
                int ret = read(evfd,buf, sizeof(buf));
                printf("%s",buf);
                close(evfd);
            }
        }
    }
        
}
