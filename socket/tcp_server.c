1 //创建、绑定、监听、accept、
  2 #include <stdio.h>
  3 #include <stdlib.h>
  4 #include <unistd.h>
  5 #include <sys/socket.h>
  6 #include <netinet/in.h>
  7 #include <arpa/inet.h>
  8 #include <string.h>
  9 int main(int argc, char* argv[])
 10 {
 11     int sock = socket(AF_INET, SOCK_STREAM, 0);
 12     struct sockaddr_in local;
 13     local.sin_family = AF_INET;
 14     local.sin_port = htons(atoi(argv[2]));
 15     local.sin_addr.s_addr = inet_addr(argv[1]);
 16 
 17     if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
 18     {
 19         perror("bind");                                                                                                                                  
 20         return 2;
 21     }
 22     if(listen(sock, 5)< 0)
 23         return 3;
 24 
 25     struct sockaddr_in client;
 26     char buf[1024];
 27     for(;;)
 28     {
 29         int new_fd = accept(sock, NULL, NULL);
 30         if(new_fd < 0)
 31             return 4;
 32         while(1)                                                                                                                                         
 33         {
 34             size_t rec = recv(new_fd, buf, sizeof(buf), 0);//最后一个是标志位
 35             if(rec == 0)//连接断开了
 36                 break;
 37             buf[rec] = '\0';
 38             printf("client say#%s\n",buf);
 39 
 40             send(new_fd, buf, strlen(buf), 0);
 41 
 42         }
 43 
 44     }
 45 }
