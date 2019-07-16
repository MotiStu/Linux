  1 #include <stdio.h>
  2 #include <stdlib.h>
  3 #include <string.h>
  4 #include <unistd.h>
  5 #include <pthread.h>
  
  6 int ticket = 100;
  7 pthreat_mutex_t mutex;
  8 void *route(void *arg)
  9 {
 10         pthread_mutex_lock(&mutex);
 11     char *id = (char*)arg;
 12     while ( 1 ) {
 13 
 14 
 15         if ( ticket > 0 ) {
 16             usleep(10000);
 17 
 18             printf("%s sells ticket:%d\n", id, ticket);
 19             ticket--;
 20             pthread_mutex_unlock(&mutex);
 21         }
 22         else
 23         {
 24             pthread_mutex_unlock(&mutex);
 25             break;
 26         }
 27     }
 28 }
 29 int main( void )
 30 {
 31     pthread_mutex_init(&mutex,NULL);
 32 
 33     pthread_t t1, t2, t3, t4;
 34     pthread_create(&t1, NULL, route, "thread 1");
 35     pthread_create(&t2, NULL, route, "thread 2");
 36     pthread_create(&t3, NULL, route, "thread 3");
 37     pthread_create(&t4, NULL, route, "thread 4");
 38 
 39     pthread_join(t1, NULL);
 40     pthread_join(t2, NULL);
 41     pthread_join(t3, NULL);
 42     pthread_join(t4, NULL);
 43 
 44     pthread_mutex_destory(&mutex);//别忘记销毁锁
 45 }

