#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;



void Usage(){
	printf("Usage ./client [ip] [port]\n");
	return;
}
int main(int argc,char* argv[]){

	if(3!=argc){

		Usage();
		return 1;
	}
	sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr(argv[1]);
	server.sin_port=htons(atoi(argv[2]));
	//1.创建套接字
	int sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
	{
		printf("error");
	}
	//2.从键盘循环的读取数据
	char buf[1024]={0};
	while(1)
	{
		socklen_t len=sizeof(server);
		printf("please enter:>   ");
		fflush(stdout);
		ssize_t read_size = read(0,buf,sizeof(buf)-1);
		//3.把读到的数据发送给服务器
		if(read_size>0)
		{
			buf[read_size-1] = 0;//read是读到换行结束的，故会多读一个字符
			sendto(sockfd,buf,strlen(buf),0,\
					(sockaddr*)&server,sizeof(server));
			ssize_t s=recvfrom(sockfd,buf,sizeof(buf)-1,0,\
					(sockaddr*)&server,&len);
			//4.把服务器发送的数据显示到屏幕上
			if(s>0)
			{
				buf[s]=0;
				printf("serevr echo:>  %s\n",buf);
			}
		}
	}
	return 0;
}
