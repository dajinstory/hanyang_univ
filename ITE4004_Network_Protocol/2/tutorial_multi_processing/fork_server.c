#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/socket.h>

#include<signal.h>
#include<sys/wait.h>

#define PORT 9000

void do_echo(int);
void sigHandler(int);

int main()
{
    int clientsock, serversock;
    struct sockaddr_in s_addr, c_addr;
    int len;

    pid_t pid;
    int state;
    struct sigaction act;

    act.sa_handler= sigHandler;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;

    state=sigaction(SIGCHLD,&act,0);

    if(state != 0)
    {
        printf("signal error\n");
        exit(1);
    }

    serversock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);

    if(bind(serversock, (struct sockaddr *) &s_addr , sizeof(s_addr))== -1)
    {
        printf("can not bind\n");
        return -1;
    }

    if(listen(serversock,5)== -1)
    {
        printf("listen fail\n");

        return -1;
    }

    while(1)
    {
        len = sizeof(c_addr);
	printf("server sock %d \n",serversock);
        if((clientsock = accept(serversock, (struct sockaddr *) &c_addr, &len))<0)
        {
            printf("accept error\n");
        } 

	pid = fork();
	
        if(pid < 0)
        {
            printf("echo server can not fork()\n");
            return -1;
        }
        else if(pid > 0)
        {
            close(clientsock);
            continue;
        }
        else if(pid == 0)
        {
            printf("child create\n");
            close(serversock);
            do_echo(clientsock);
        }

    }
}

void do_echo(int clientsock)
{
    int n;
    char recvBuffer[4096];

   
    while((n=read(clientsock, recvBuffer, sizeof(recvBuffer)))!=0)
    {
	if(n > 0)
	{
        	printf("child(%d)\n",n);
	}
        write(clientsock,recvBuffer,n);
    }
    exit(1);
}

void sigHandler(int sig)
{
    int pid;
    int status;

    pid = wait(&status);
    printf("pid[%d] terminate\n",pid);
}
