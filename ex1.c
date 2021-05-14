//
// ex_model.c
//
//1. Generați 100.000 de numere aleatoare și calculați suma lor utilizând 100 thread-uri.
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

// maximum number of threads
#define MAX_THR 100
#define MAX_NR 100000
#define MAX_LIM 1000
// shared variable
int sum=0;
int nr[MAX_NR];

// lock object
pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;

// thread start routine
void* do_sum(void* a)
{
	int k = *(int*)a;
	//printf("Eu sunt thread-ul %d\n", k);
	int min=k*MAX_LIM;
	int max=min+MAX_LIM;
	int i;
	for(i=min;i<max;i++)
	{
		pthread_mutex_lock(&mtx);
		sum+=nr[i];
		pthread_mutex_unlock(&mtx);
	}
	return NULL;
}



int main(int argc, char* argv[])
{
	pthread_t t[MAX_THR];
	
	int i;
	srand(time(0));
	for(i=0;i<MAX_NR;i++)
	{
		nr[i]=rand()%10+1;
	}

	for (i = 0; i < MAX_THR; i++)
	{
		int* k=(int*)malloc(sizeof(int));
		*k=i;
		pthread_create(&t[i], NULL, do_sum, k);
	}

	for (i = 0; i < MAX_THR; i++)
	{
		pthread_join(t[i], NULL);
	}
	printf("SUMA :%d\n",sum);
	int sum2=0;
	for(i=0;i<MAX_NR;i++)
	{
		sum2=sum2+nr[i];
	}
	printf("SUMA2: %d\n",sum2);
	return 0;
}
