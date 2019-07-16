#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define CONSUMERS_COUNT 2//消费者
#define PRODUCERS_COUNT 2//生产者
struct msg//物品
{
	struct msg *next;
	int num;
};

struct msg *head = NULL;
pthread_cond_t cond;//信号量
pthread_mutex_t mutex;//互斥锁
pthread_t threads[CONSUMERS_COUNT+PRODUCERS_COUNT];

void *consumer(void *p)//消费者
{
	int num = *(int*)p;
	free(p);
	struct msg *mp;
	for ( ; ; ) 
	{
	pthread_mutex_lock(&mutex);
	while ( head == NULL ) //刚开始head==NULL，等待生产者生产，就会改变head
	{
		printf("%d begin wait a condition...\n", num);
		pthread_cond_wait(&cond, &mutex);//不满足条件，等待
	}
		printf("%d end wait a condition...\n", num);
		printf("%d begin consume product...\n", num);
		mp = head;//用mp把头一接收，然后头后移
		head = mp->next;
		pthread_mutex_unlock(&mutex);//东西买完，就可以解锁了
		
		printf("Consume %d\n", mp->num);
		free(mp);//释放mp
		printf("%d end consume product...\n", num);
		sleep(rand()%5);
	}
}
void *producer(void *p)//生产者
{
	struct msg *mp;
	int num = *(int*)p;
	free(p);
	for ( ; ; ) 
	{
		printf("%d begin produce product...\n", num);
		mp = (struct msg*)malloc(sizeof(struct msg));
		mp->num = rand()%1000 + 1;//num是编号的意思，没有啥
		printf("produce %d\n", mp->num);
		
		pthread_mutex_lock(&mutex);//在这加锁，生产者生产之后需要将货物串起来，加锁，否则货物就会少，资源泄露
		mp->next = head;//头插货物
		head = mp;
		printf("%d end produce product...\n", num);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
		sleep(rand()%5);
	}
}
int main( void )
{
	srand(time(NULL));
	pthread_cond_init(&cond, NULL);
	pthread_mutex_init(&mutex, NULL);
	int i;
	for(i=0; i<CONSUMERS_COUNT; i++) //创建消费者线程
	{
		int *p = (int*)malloc(sizeof(int));
		*p = i;
		pthread_create(&threads[i], NULL, consumer, (void*)p);
	}
	for(i=0; i<PRODUCERS_COUNT; i++) //创建生产者线程
	{
		int *p = (int*)malloc(sizeof(int));
		*p = i;
		pthread_create(&threads[CONSUMERS_COUNT+i], NULL, producer, (void*)p);
	}
	for (i=0; i<CONSUMERS_COUNT+PRODUCERS_COUNT; i++)
		pthread_join(threads[i], NULL);
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
}
