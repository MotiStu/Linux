 1 #include <iostream>
  2 #include <sys/socket.h>
  3 #include <netinet/in.h>
  4 #include <arpa/inet.h>
  5 #include <stdlib.h>
  6 #include <string.h>
  7 #include <poll.h>
  8 #include <unistd.h>
  9 using namespace std;
 10 typedef struct pollfd pollfd;
 11 
 12 void Init(pollfd* fd_list, int size)
 13 {
 14     for (int i = 0; i < size; ++i)
 15     {
 16         fd_list[i].fd = -1;
 17         fd_list[i].events = 0;
 18         fd_list[i].revents = 0;
 19     }
 20 }
 21 void Add(int fd, pollfd* fd_list, int size)
 22 {                                                                                                                                                        
 23     for (int i = 0; i < size; ++i)
 24     {
 25         if (fd_list[i].fd == -1)
 26         {
 27             fd_list[i].fd = fd;
28             fd_list[i].events = POLLIN;
 29             break;
 30         }
 31     }
 32 }
 33 
 34 int main(int argc, char* argv[])
 35 {
 36     if(argc != 3)
 37     {
 38         cout<<"Usage: ip port"<<endl;
 39         return 1;
 40     }
 41 
 42     struct sockaddr_in addr;
 43     addr.sin_family = AF_INET;
 44     addr.sin_addr.s_addr = inet_addr(argv[1]);
 45     addr.sin_port = htons(atoi(argv[2]));
 46 
 47     int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
 48     if(listen_fd < 0)
 49     {
 50         cout<<"sokcet error..."<<endl;
 51         return 1;
 52     }
 53 
 54     if(bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
 55     {
 56         cout<<"bind error..."<<endl;
 57         return 1;
 58     }
 59 
 60     if(listen(listen_fd, 5) < 0)
 61     {
 62         cout<<"listen error..."<<endl;
 63     }
 64 
 65     pollfd fd_list[1024];               
 66     Init(fd_list, sizeof(fd_list) / sizeof(pollfd));
 67     Add(listen_fd, fd_list, sizeof(fd_list) / sizeof(pollfd));//将新的套接字加入进去
 68     for (;;)
 69     {
 70         int ret = poll(fd_list, sizeof(fd_list) / sizeof(pollfd), 6000);
 71         if (ret < 0) {
 72             perror("poll");
 73             continue;
 74             
 75         }
 76         if (ret == 0) {
 77             printf("poll timeout\n");
 78             continue;
 79             
 80         }
 81         for (size_t i = 0; i < sizeof(fd_list) / sizeof(pollfd); ++i)
 82         {
 83             if (fd_list[i].fd == -1) 
 84             {
 85                 continue;
 86             }
 87             if (!(fd_list[i].revents & POLLIN))
 88             {
 89                 continue;
 90             }
 91             if (fd_list[i].fd == listen_fd)
 92             {
 93                 // 处理listen_fd 的情况.
 94                 struct sockaddr_in client_addr;
 95                 socklen_t len = sizeof(client_addr);
 96                 int connect_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &len);//accept 的是真正来收发数据的
 97                 if (connect_fd < 0)
 98                 {
 99                     perror("accept");
100                     continue;
101                 }
102                 cout<<"new client join:"<<inet_ntoa(client_addr.sin_addr)\
103                     <<" "<<ntohs(client_addr.sin_port)<<endl;
104                 Add(connect_fd, fd_list, sizeof(fd_list) / sizeof(pollfd));//将accept的加入到poll中
105             }
106             else  
107             {
108                 // 处理connect_fd 的情况.
109                 char buf[1024] = {0};
110                 size_t read_size = read(fd_list[i].fd, buf, sizeof(buf) - 1);
111                 if (read_size < 0)
112                 {
113                     perror("read");
114                     continue;
115                 }
116                 if (read_size == 0)//这里是客户端退出了
117                 {
118                     cout<<"client goodbye"<<endl;
119                     //close(fd_list[i].fd);
120                     fd_list[i].fd = -1;
121                     return 1;
122                 }
123                 cout<<"client say: "<<buf<<endl;
124                 write(fd_list[i].fd, buf, strlen(buf));
125             }   
126         }   
127     }       
128     return 0;
129 }               
130       
