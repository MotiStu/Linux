  1 #include<stdio.h>
  2 #include<string.h>
  3 #include<stdlib.h>
  4 #include<unistd.h>
  5 #include<sys/socket.h>
  6 #include<netinet/in.h>
  7 #include<arpa/inet.h>
  8 
  9 int main(int argc,char *argv[])
 10 {
 11     if(argc!=3)
 12     {
 13         perror("Usage: ip port\n");
 14         return 1;
 15 
 16     }
 17     int sockfd=socket(AF_INET,SOCK_STREAM,0);
 18     if(sockfd<0)
 19     {
 20         perror("socket\n");
 21         return 2;
 22 
 23     }                                                                                                                                                    
 24 
 25     char buf[128];
 26     struct sockaddr_in server;
 27     server.sin_family=AF_INET;
 28     server.sin_port=htons(atoi(argv[2]));
  29     server.sin_addr.s_addr=inet_addr(argv[1]);
 30     if(connect(sockfd,(struct sockaddr*)&server,sizeof(server))<0)
 31     {
 32         perror("connect\n");                                                                                                                             
 33         return 3;
 34 
 35     }
 36 
 37     printf("connetc success\n");
 38 
 39     while(1)
 40     {
 41         memset(buf,0,sizeof(buf));
 42         ssize_t r=read(0,buf,sizeof(buf));
 43         if(r<0)
 44         {
 45             printf("read error\n");
 46             continue;
 47 
 48         }
 49         buf[r-1]=0;
 50         if(send(sockfd,buf,strlen(buf),0)==0)
 51             continue;
 52         memset(buf,0,sizeof(buf));
 53         ssize_t rec=recv(sockfd,buf,sizeof(buf),0);
 54         printf("server > %s\n",buf);
 55 
 56     }
 57     close(sockfd);//use signal to delet
 58     return 0;                                                                                                                                            
 59 
 60 }
