#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#define RANGE 200

int N = 0;
int M = 0;
int* numbers = NULL;
int thrNum;
int flag = 0;
int maxim = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condVar = PTHREAD_COND_INITIALIZER;
pthread_barrier_t barrier;

void* produce(void* a)
{
    pthread_mutex_lock(&mutex);
    while (flag == 1)
    {
	    pthread_cond_wait(&condVar, &mutex);
    }
    srand(time(0));
    int i;
    for (i = 0; i < N; i++)
        numbers[i]=rand()%255+1;

    flag = 1;
    pthread_cond_signal(&condVar);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* consume(void* idPtr)
{
    int id = *(int*)idPtr;

    pthread_mutex_lock(&mutex);
    while (flag == 0)
        pthread_cond_wait(&condVar, &mutex);
    pthread_mutex_unlock(&mutex);

    int localSum = 999;
    int start = id * RANGE;
    int end = (id + 1) * RANGE;

    int i;
    for (i = start; i < end; i++)
    {
	    if(numbers[i]%11==0&&numbers[i+1]%11==0)
	    {
		    if(localSum>numbers[i]+numbers[i+1])
		    {
			    localSum=numbers[i]+numbers[i+1];
		    }
	    }
    }
    if(localSum!=999)
    {
    	printf("Thread %d, range %d->%d, sum=%d:\n", id, start, end, localSum);
    }

    pthread_barrier_wait(&barrier);

    pthread_mutex_lock(&mutex);
    if(localSum>maxim && localSum!=999)
    {
	    maxim=localSum;
    }
    pthread_mutex_unlock(&mutex);

    free(idPtr);
    return NULL;
}

int main()
{
    printf("Introduceti N:");
    scanf("%d", &N);

    numbers = malloc(sizeof(int) * N);

    pthread_t producer;
    pthread_create(&producer, NULL, produce, NULL);

    thrNum = N / RANGE;

    pthread_t* consumers = malloc(sizeof(pthread_t) * thrNum);

    int i;

    pthread_barrier_init(&barrier, NULL, thrNum);

    for (i = 0; i < thrNum; i++)
    {
        int* idPtr = malloc(sizeof(int));
        *idPtr = i;
        pthread_create(&consumers[i], NULL, consume, idPtr);
    }
    pthread_join(producer, NULL);

    for (i = 0; i < thrNum; i++)
        pthread_join(consumers[i], NULL);

    printf("Maxim sum :%d\n", maxim);

    pthread_barrier_destroy(&barrier);
    free(consumers);
    free(numbers);

    return 0;
}
