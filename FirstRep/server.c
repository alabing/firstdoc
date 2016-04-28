#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<netdb.h>
#define PORT 2500
#define buffer_size 200
void *mysend(int fd)
{
	char buf[buffer_size];
	int nbytes;
	while(1)
	{	
		fgets(buf,30,stdin);
		nbytes=write(fd,buf,buffer_size);
		if(nbytes<0)
		{	
			printf("write error!\n");
			exit(1);
		}
	usleep(1000);
	}
}
void *myrecv(int fd)
{
	char buf[buffer_size];
	int nbytes;
	while(1)
	{	
		nbytes=read(fd,buf,buffer_size);	
		if((nbytes==0)||(nbytes==-1))
		{
			if(nbytes==0)
			{
				printf("the file is empty...\n");
				exit(1);
			}
			else
			{	
				printf("read error...\n");
				exit(1);
			}
		}
		else
			printf("recvfrom client:%s",buf);
	}
	
}
int main()
{
	int fd;
	int connfd;
	int ret,ret1;
	int len;
	char buf[buffer_size];
	struct sockaddr_in addr;
	int addrlen=sizeof(struct sockaddr_in);
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	pthread_t id1,id2;     //
	
	//创建套接字
	fd=socket(AF_INET,SOCK_STREAM,0);
	if(fd==-1)
	{
		printf("socket fd create error...\n");
		exit(1);
	}
	else
	{
		printf("socket fd:%d create success\n",fd);
		//printf("flag:\n");
	}
	//printf("222222222\n");
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	server_addr.sin_port=htons(PORT);
	//bind
	//printf("1111111111111111\n");
	if(bind(fd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))<0)
	{
		printf("bind error!\n");
	}
	else
	{
		printf("bind successfully!local port:%d\n",PORT);
	}
	//listen
	if(listen(fd,5)<0)
		printf("error:listening on socket\n");
	else
		printf("success:listening on socket\n");
//	while(1)
//	{
		//accept
		int addrlen_ = sizeof (struct sockaddr) ;
		connfd=accept(fd,(struct sockaddr*)(&client_addr),&addrlen_);
		printf("connfd:%d\n",connfd);
		if(connfd==-1)
			printf("accept error\n");
		else
			printf("accept success\n");
		//send pthread
		
		ret=pthread_create(&id1,NULL,mysend,connfd);
		if(ret)
		{
			printf("create pthread error!\n");
			return 1;
		}
		//recv pthread
		ret1=pthread_create(&id2,NULL,myrecv,connfd);
		if(ret1)
		{	
			printf("create pthread error!\n");
			return 1;
		}
		void *tret;
		pthread_join(id1,&tret);
		pthread_join(id2,&tret);
		
		//mysend(connfd);
		//myrecv(connfd);
		close(connfd);
		return 0;
//	}
	
}
