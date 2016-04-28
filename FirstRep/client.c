#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<sys/socket.h>
#include<pthread.h>
#define PORT 2500
#define REMOTE_IP "127.0.0.1"
#define buffer_size 200
void mysend(int fd)
{
	char buf[buffer_size];
	int nbytes;
	while(1)
	{	
		memset(buf,0,buffer_size);
		fgets(buf,30,stdin);
		nbytes=write(fd,buf,buffer_size);
		if(nbytes<0)
		{
			printf("write error\n");
			exit(1);
		}

	}
}
void myrecv(int fd)
{
	char buf[buffer_size];
	int nbytes;
	while(1)	
	{
		memset(buf,0,buffer_size);
		nbytes=read(fd,buf,buffer_size);
		if((nbytes==0)||(nbytes==-1))
		{
			printf("read error");
			exit(1);
		}
		else
		{
			printf("recvfrom server:%s",buf);
		}
	}
}

int main()
{
	char i;
	int ret,ret1;
	pthread_t id1,id2;
	int nbytes;
	char buf[buffer_size];
	struct sockaddr_in server_addr;
	//struct sockaddr_in client_addr;
	//建立套接字
	int fd;
	fd=socket(AF_INET,SOCK_STREAM,0);
	if(fd<0)
	{
		printf("socket error!\n");
		exit(1);
	}	
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(PORT);
	server_addr.sin_addr.s_addr=inet_addr(REMOTE_IP);
	int addrlen;
	addrlen=sizeof(struct sockaddr);
	if(connect(fd,(struct sockaddr*)(&server_addr),addrlen)==-1)
	{
		printf("connect error!\n");
		exit(1);
	}
	else
	{
		printf("connect success!\n");
	}
//	while(1)
//	{
		printf("another pthread create!\n");
		ret=pthread_create(&id1,NULL,mysend,fd);
		if(ret)
		{
			printf("create pthread error!\n");
			return 1;
		}
		ret1=pthread_create(&id2,NULL,myrecv,fd);
		if(ret1)
		{
			printf("create pthread error\n");
			return 1;
		}
		void *tret;
		pthread_join(id1,&tret);
		pthread_join(id2,&tret);
		printf("message:%s\n",buf);
		close(fd);
		return 1;
//	}
}
