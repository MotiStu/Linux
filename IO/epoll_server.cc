 1 //这是一个epoll的服务器程序
  2 #include <iostream>
  3 #include <sys/epoll.h>
  4 #include <sys/socket.h>
  5 #include <netinet/in.h>
  6 #include <unistd.h>
  7 #include <string.h>
  8 #include <stdlib.h>
  9 #include <arpa/inet.h>
 10 using namespace std;
 11 void ProcessConnect(int listen_fd, int epoll_fd)//连接到来
 12 {
 13     struct sockaddr_in client_addr;
 14     socklen_t len = sizeof(client_addr);
 15     int connect_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &len);
 16     if(connect_fd < 0)
 17     {
 18         cout<<"accept error...";
 19         return ;
 20     }
 21     cout<<"new client connect: "<<inet_ntoa(client_addr.sin_addr)<<" "\
 22         <<ntohs(client_addr.sin_port)<<endl;
 23 
 24     struct epoll_event ev;
 25     ev.data.fd = connect_fd;
 26     ev.events = EPOLLIN;
 27 
 28     int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connect_fd, &ev);
 29     if(ret < 0)
 30     {
 31         cout<<"ctl error..."<<endl;
 32         return ;
 33     }
 34     return ;
 35 }
 36 void ProcessRequest(int connect_fd, int epoll_fd)                                                                                                          
 37 {
 38     char buf[1024];
 39     ssize_t read_size = read(connect_fd, buf, sizeof(buf)-1);
 40     if(read_size < 0)
 41     {
 42         cout<<"read error...."<<endl;
 43         return ;
 44     }
 45     if(read_size == 0)
 46     { 
 47         close(connect_fd);
 48         epoll_ctl(epoll_fd, EPOLL_CTL_DEL, connect_fd, NULL);
 49         cout<<"client close..."<<endl;
 50         return ;
 51     }
 52     cout<<"client say: "<<buf<<endl;
 53     write(connect_fd, buf, strlen(buf));
 54 }       
 55 int main(int argc, char* argv[])
 56 {   
 57     if(argc!=3)
 58         cout<<"Use: IP  port..."<<endl;
 59 
 60     struct sockaddr_in addr;
 61     addr.sin_family = AF_INET;
 62     addr.sin_addr.s_addr = inet_addr(argv[1]);
 63     addr.sin_port = htons(atoi(argv[2]));
 64     
 65     int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
 66     if(listen_fd < 0)
 67     {   
 68         cout<<"socket error..."<<endl;
 69         return 1;
 70     }
 71     if(bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
 72     {
 73         cout<<"bind error..."<<endl;
 74         return 1;
 75     }
 76     if(listen(listen_fd, 5) < 0)
 77     {
 78         cout<<"listen error..."<<endl;
 79         return 1;
 80     }
 81     
 82     //下面就需要用到多路转接之epoll了
 83     int epoll_fd = epoll_create(10);
 84     //这里是创建了epoll模型,就绪队列，红黑树，回调机制 
 85     if(epoll_fd < 0)
 86     {   
 87         cout<<"epoll_create error..."<<endl;
 88         return 1;
 89     }
 90     struct epoll_event event;
 91     event.events = EPOLLIN;//关注读事件
 92     event.data.fd = listen_fd;
 93     int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event);
 94     if(ret < 0)
 95     {
 96         cout<<"epoll_ctl error...."<<endl;
 97         return 1;
 98     }
 99     
100     for(;;)
101     {
102         struct epoll_event events[10];
103         int size = epoll_wait(epoll_fd, events, sizeof(events)/sizeof(events[0]), 5000);
104         if(size < 0)
105         {
106             cout<<"epoll_wait...."<<endl;
107             continue;
108         }
109         if(size == 0)
110         {
111             cout<<"epoll_wait timeout...."<<endl;
112             continue;
113         }
114         for(int i=0;i<size;i++)
115         {
116             if(!(events[i].events & EPOLLIN))
117                 continue;
118             if(events[i].data.fd == listen_fd)
119                 ProcessConnect(listen_fd, epoll_fd);//连接到来
120             else
121                 ProcessRequest(events[i].data.fd, epoll_fd);
122         }
123     }   
124     return 0;
125 }   
126     
