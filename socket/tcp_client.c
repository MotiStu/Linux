 1 #include <stdlib.h>
  2 #include <stdio.h>
  3 #include <sys/socket.h>
  4 #include <arpa/inet.h>
  5 #include <string.h>
  6  #include <unistd.h>//接口通常都是大量针对系统调用的封装（英语：wrapper functions），如 fork、pipe 以及各种 I/O 原语（read、write、close 等等）。
  7 int main(int argc ,char* argv[])
  8 {
  9     int sock  = socket(AF_INET, SOCK_STREAM, 0);
 10     if(sock < 0)                                                                                                                                         
 11     {
 12         return 1;
 13     }
 14     struct sockaddr_in server;
 15     server.sin_family = AF_INET;
 16     server.sin_port = htons(atoi(argv[2]));
 17     server.sin_addr.s_addr = inet_addr(argv[1]);
 18 
 19     if(connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0)//连接
 20     {
 21         return 2;
 22     }
 23 
 24     char buf[1024] = {0};
 25     for(;;)
 26     {
 27         memset(buf,0,sizeof(buf));
 28         ssize_t len = read(0, buf, sizeof(buf));
 29         if(len == 0)
 30         {
 31             continue;
 32         }                                                                                                                                                
 33         buf[len -1] = 0;
 34         if(send(sock, buf, strlen(buf), 0) == 0)
 35             continue;
 36         memset(buf,0,sizeof(buf));
 37         recv(sock, buf, strlen(buf), 0);
 38     }
 39     close(sock);
 40     return 0;
 41 
 42 
 43 
 44 }
