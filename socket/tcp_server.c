    #include<stdio.h>
  2 #include<string.h>
  3 #include<stdlib.h>
  4 #include<unistd.h>
  5 #include<sys/socket.h>
  6 #include<netinet/in.h>
  7 #include<arpa/inet.h>
  8 #include<sys/types.h>
  9 
 10 int main(int argc,char *argv[])
 11 {
 12     if(argc!=3)
 13     {                                                                                                                                                    
 14         perror("argc\n");
 15         return 1;
 16 
 17     }
 18     int sockfd=socket(AF_INET,SOCK_STREAM,0);
 19     if(sockfd<0)
 20     {
 21         perror("socket\n");
 22         return 2;
 23 
 24     }
 25 
 26     char buf[128];
 27     struct sockaddr_in local;
 28     local.sin_family=AF_INET;
 29     local.sin_port=htons(atoi(argv[2]));
 30     local.sin_addr.s_addr=inet_addr(argv[1]);
 31 
 32     if(bind(sockfd,(struct sockaddr*)&local,sizeof(local))<0)                                                                                            
 33     {
 34         perror("bind");
 35         return 2;
 36 
 37     }
 38 
 39     if(listen(sockfd,5)<0)
 40     {
 41         perror("listen\n");
 42         return 5;
 43 
 44     }
 45     while(1)
 46     {
 47         int new_sockfd=accept(sockfd,NULL,NULL);
 48         if(new_sockfd<0)
 49         {
 50             perror("accept\n");
 51             return 6;
 52 
 53         }
 54                 struct sockaddr_in client;
 55 
 56                 while(1)
 57                 {
 58                                                                                                                                                          
 59                     socklen_t len=sizeof(client);
 60                     size_t rec=recv(new_sockfd,buf,sizeof(buf),0);
 61                     if(rec<0)
 62                     {
 63                         printf("recv error\n");
 64                         return 4;
 65 
 66                     }
 67                     if(rec==0)
 68                     {
 69                         break;;
 70 
 71                     }
 72 
 73                     buf[rec]='\0';
 74                     printf("[%s,%d] > %s\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port),buf);
 75                     send(new_sockfd,buf,strlen(buf),0);
 76 
 77                 }
 78                 close(new_sockfd);//use signal to delet
 79 
 80             }
 81 
 82 
 83     close(sockfd);
 84     return 0;                                                                                                                                            
 85 
 86 }
~                                                                                                                                                            
~        
