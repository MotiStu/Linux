  1 //这是client                                                                                                                                             
  2 #include <stdio.h>
  3 #include <sys/types.h>
  4 #include <sys/stat.h>
  5 #include <fcntl.h>
  6 #include <unistd.h>
  7 #include <stdlib.h>
  8 #include <string.h>
  9 #define ERR_EXIT(m) \
 10     do{\
 11     perror(m);\
 12     exit(EXIT_FAILURE);\
 13     }while(0)
 14 int main()
 15 {
 16     int wfd = open("mypipe", O_WRONLY);
 17     if(wfd < 0){
 18         ERR_EXIT("open");
 19 
 20     }
 21     char buf[1024];
 22     while(1){
 23         buf[0] = 0;
 24         printf("Please Enter# ");
 25         fflush(stdout);
 26         ssize_t s = read(0, buf, sizeof(buf)-1);
 27         if(s > 0 ){
 28             buf[s] = 0;
 29             write(wfd, buf, strlen(buf));
 30             
 31         }else if(s <= 0){
 32             ERR_EXIT("read");
 33             
 34         }
 35         
 36     }
 37     close(wfd);
 38     return 0;
 39     
 40 }
 41     
