//这是一个select的服务器
  2 #include <iostream>
  3 #include <sys/select.h>
  4 #include <sys/types.h>
  5 #include <string.h>
  6 #include <sys/socket.h>
  7 #include <netinet/in.h>
  8 #include <arpa/inet.h>
  9 #include <stdlib.h>
 10 #include <unistd.h>
 11 using namespace std;
 12                                                                                                                                                          
 13 void Init(int* fd_list, int size)
 14 {
 15     for(int i=1;i<size;i++)
 16     {
 17         fd_list[i]=-1;
 18     }
 19 }
 20 void Reload(int listen_fd, int* connect_list, int size, \
 21             fd_set* read_fds, int* max_fd)
 22 {
 23     FD_ZERO(read_fds);
 24     FD_SET(listen_fd, read_fds);
 25     int max = listen_fd;
 26     for(int i=0;i<size;i++)
 27     {
 28         if(connect_list[i] != -1)
 29         {
 30             FD_SET(connect_list[i], read_fds);
 31             if(connect_list[i]>max)
 32                 max = connect_list[i];
 33         }
 34     }
 35     *max_fd = max;
 36 }
 37 void Add(int fd, int* connect_list, int size)
 38 {
 39     for(int i=0;i<size;i++)
 40     {
 41         if(connect_list[i] == -1)
 42         {
 43             connect_list[i] = fd;
 44             break;
 45         }
 46     }       
 47     return ;
 48 }   
 49 int main(int argc, char* argv[])
 50 {   
 51     if(argc != 3)
 52     {
53         cout<<"Usage: "<<" ip, port"<<endl;
 54         return 1;
 55     }       
 56             
 57     struct sockaddr_in addr;
 58     addr.sin_family = AF_INET;
 59     addr.sin_addr.s_addr = inet_addr(argv[1]);
 60     addr.sin_port = htons(atoi(argv[2]));
 61 
 62     int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
 63     if(listen_fd < 0 )
 64     {
 65         cout<<"socket error.."<<endl;
 66         return 1;
 67     }   
 68             
 69     int ret = bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr));
 70     if(ret < 0)
 71     {       
 72         cout<<"bind error..."<<endl;
 73         return 1;
 74     }
 75 
 76     ret = listen(listen_fd, 5);
 77     if(ret < 0)
78     {   
 79         cout<<"listen error..."<<endl;
 80         return 1;
 81     }       
 82     
 83     fd_set read_fds;
 84     int fd_list[1024];
 85     Init(fd_list, sizeof(fd_list)/sizeof(int));
 86     for(;;)
 87     {
 88         int max_fd = listen_fd;
 89         Reload(listen_fd, fd_list, sizeof(fd_list)/sizeof(int), \
 90                &read_fds, &max_fd);
 91         //cout<<"before select "<<endl;
 92         int ret = select(max_fd+1, &read_fds, NULL, NULL, NULL);
 93         //cout<<"after select "<<endl;
 94         if(ret < 0)
 95         {
 96             cout<<"select error ..."<<endl;
 97             continue;
 98         }
 99         if(ret == 0)
100         {
101             cout<<"select tomeuout"<<endl;
102         }
103         if(FD_ISSET(listen_fd, &read_fds))
104         {
105             struct sockaddr_in client_addr;
106             socklen_t len = sizeof(client_addr);
107             int connect_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &len);
108             if(connect_fd < 0)
109             {
110                 cout<<"accept error..."<<endl;
111                 continue;
112             }
113             cout<<"a client join: "<<inet_ntoa(client_addr.sin_addr)<<\
114                " "<< ntohs(client_addr.sin_port)<<endl;
115             Add(connect_fd, fd_list, sizeof(fd_list)/sizeof(int));
116         }
117         //处理connect_fd
118         for(size_t i=0;i<sizeof(fd_list)/sizeof(int);i++)
119         {
120             if(fd_list[i] ==-1)
121                 continue;
122             if(!FD_ISSET(fd_list[i], &read_fds))
123                 continue;
124         
125             char buf[1024];
126             ssize_t read_size = read(fd_list[i], buf, sizeof(buf)-1);
127             if(read_size < 0)
128             {
129                 cout<<"read error..."<<endl;
130                 continue;
131             }
132             if(read_size == 0)
133             {
134                 cout<<"client out..."<<endl;
135                // close(fd_list[i]);
136                 fd_list[i] = -1;
137                 return 1;
138             }
139                
140             write(fd_list[i], buf, strlen(buf));
141             cout<<"client say: "<<buf<<endl;
142         
143         }
144         
145     }       
146     return 0;   
147 }           
148                 
