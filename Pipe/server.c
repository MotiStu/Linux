  1 //用命名管道实现对话  
  2 //这是server
  3 #include <stdio.h>                                                                                                                                       
  4 #include <sys/types.h>
  5 #include <sys/stat.h>
  6 #include <fcntl.h>
  7 #include <unistd.h>
  8 #include <stdlib.h>
  9 #define ERR_EXIT(m) \
 10     do{\
 11     perror(m);\
 12     exit(EXIT_FAILURE);\
 13     }while(0)
 14 int main()
 15 {
 16     umask(0);
 17     if(mkfifo("mypipe", 0644) < 0){
 18         ERR_EXIT("mkfifo");
 19 
 20     }
 21     int rfd = open("mypipe", O_RDONLY);
 22     if(rfd < 0){
 23         ERR_EXIT("open");
 24 
 25     }
 26     char buf[1024];
 27     while(1){
 28         buf[0] = 0;
 29         printf("Please wait...\n");
 30         ssize_t s = read(rfd, buf, sizeof(buf)-1);
 31         if(s > 0 ){
 32             buf[s-1] = 0;
 33             printf("client say# %s\n", buf);
 34             
 35         }else if(s == 0){
 36             printf("client quit, exit now!\n");
 37             exit(EXIT_SUCCESS);
 38             
 39         }else{
 40             ERR_EXIT("read");
 41             
 42         }
 43         
 44     }
 45     close(rfd);
 46     return 0; 
 47     
 48 }       
