//这是一个简单的卖票的简单逻辑，有100张票，4个销售（线程），在没有加锁的时候四个线程可以并发的对ticket进行操作，这样就会出错

  1 #include <stdio.h>
  2 #include <stdlib.h>
  3 #include <string.h>
  4 #include <unistd.h>
  5 #include <pthread.h>
  6 int ticket = 100;
  7 void *route(void *arg)
  8 {
  9     char *id = (char*)arg;
 10     while ( 1 ) {
 11         if ( ticket > 0 ) {
 12             usleep(50000);
 13 
 14             printf("%s sells ticket:%d\n", id, ticket);
 15             ticket--;
 16         }
 17         else
 18         {
 19             break;
 20         }
 21     }
 22 }
 23 int main( void )
 24 {
 25     pthread_t t1, t2, t3, t4;
 26     pthread_create(&t1, NULL, route, "thread 1");
 27     pthread_create(&t2, NULL, route, "thread 2");
 28     pthread_create(&t3, NULL, route, "thread 3");
 29     pthread_create(&t4, NULL, route, "thread 4");
 30 
 31     pthread_join(t1, NULL);
 32     pthread_join(t2, NULL);
 33     pthread_join(t3, NULL);
 34     pthread_join(t4, NULL);
 35 }
 
