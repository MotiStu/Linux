#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;



void Usage(){

	    printf("Usage ./server [ip] [port]\n");
		    return;
}
int main(int argc,char*argv[]){

	if(3!=argc){

		Usage();
		return 1;
	}
	sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(atoi(argv[2]));
	local.sin_addr.s_addr=inet_addr(argv[1]);
	//1.创建套接字
	int sockfd= socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
		printf("error");	
	//2.进行绑定
	int ret= bind(sockfd,(sockaddr*)&local,sizeof(local));
	if(ret<0)
		printf("error");	
	//3.循环的把客户端发送的数据，给客户端发送回去
	char buf[1024]={
		0};
	sockaddr_in client;
	while(1){

		socklen_t len=sizeof(client);
		ssize_t s=recvfrom(sockfd,buf,sizeof(buf)-1,0,\
				(sockaddr*)&client,&len);
		if(s>0) {

			buf[s]=0;
			printf("client [%s: %d] say:> %s\n",inet_ntoa(client.sin_addr),\
					ntohs(client.sin_port),buf);
			sendto(sockfd,buf,strlen(buf),0,\
					(sockaddr*)&client,sizeof(client));
		}
	}
	return 0;
}
