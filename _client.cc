 1 #include <iostream>
  2 #include <sys/socket.h>
  3 #include <unistd.h>
  4 #include <netinet/in.h>
  5 #include <arpa/inet.h>
  6 #include <string.h>
  7 #include <stdlib.h>
  8 using namespace std;
  9 int main(int argc, char* argv[])
 10 {
 11     if(argc!=3)
 12     {
 13         cout<<"Usage: "<<endl;
 14         return 1;
 15     }
 16     struct sockaddr_in addr;
 17     addr.sin_family = AF_INET;
 18     addr.sin_addr.s_addr = inet_addr(argv[1]);
 19     addr.sin_port = htons(atoi(argv[2]));
 20 
 21     int fd = socket(AF_INET, SOCK_STREAM, 0);
 22     if(fd < 0)
 23     {
 24         cout<<"socket error..."<<endl;
 25         return 1;
 26     }                                                                                                                                                    
 27     int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
28     if(ret < 0)
 29     {
 30         cout<<"connect erroc"<<endl;
 31         return 1;
 32     }
 33     for(;;)
 34     {
 35         cout<<":> ";
 36         fflush(stdout);
 37         char buf[1024] = {0};
 38         read(0, buf, sizeof(buf)-1);
 39         ssize_t write_size = write(fd, buf, strlen(buf));
 40         if(write_size < 0)
 41         {
 42             cout<<"write error..."<<endl;
 43             continue;
 44         }
 45 
 46         ssize_t read_size = read(fd, buf, sizeof(buf)-1);
 47         if(read_size < 0)
 48         {
 49             cout<<"read error..."<<endl;
 50             continue;
 51         }
 52         if(read_size == 0)
53         {
 54             cout<<"server close..."<<endl;
 55             break;
 56         }
 57         //cout<<"server say: "<<buf<<endl;
 58     }
 59     close(fd);
 60     return 0;
 61 }       
 62         
