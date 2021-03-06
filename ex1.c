#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// maximum number of threads
#define MAX_SIZE 6000
#define MAX_NUM 200
// shared variable
int numbers[MAX_SIZE];
int sum=0;
int full=0;
// lock object
pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv=PTHREAD_COND_INITIALIZER;

// thread start routine
void* f1(void* a)
{
	int N = *(int*)a;
	int i=0;
	free(a);
	sleep(5);

	srand(time(NULL));
	for (i = 0; i < N; i++)
	{
		pthread_mutex_lock(&mtx);
		while(full==1)
		{
			pthread_cond_wait(&cv,&mtx);
		}
		numbers[i]=rand()%255+1;
		pthread_cond_signal(&cv);
		pthread_mutex_unlock(&mtx);
	}
	//free(a);
	//sleep(2);

	full=1;
	return NULL;
}

void* f2(void* a)
{
	int k = *(int*)a;
	int i=0;
	free(a);
	int min=k*MAX_NUM;
	int max=min+MAX_NUM;
	for (i = min; i < max; i++)
	{
		pthread_mutex_lock(&mtx);
		while(full==0)
		{
			pthread_cond_wait(&cv,&mtx);
		}
		sum+=numbers[i];
		pthread_cond_signal(&cv);
		pthread_mutex_unlock(&mtx);
	}
	
	return NULL;
}


int main(int argc, char* argv[])
{
	//citesc valoare lui N
	int N=0;
	printf("Dati N: ");
	scanf("%d",&N);

	//creez threadul producator
	pthread_t t1;
	int *n=(int*)malloc(sizeof(int));
	*n=N;
	pthread_create(&t1,NULL,f1,n);


	//calculez numarul de thread-uri de tip consumator
	int nthr=(int)(N/MAX_NUM);

	pthread_t t2[nthr];

	int i=0;
	for (i = 0; i < nthr; i++)
	{
		int* k=(int*)malloc(sizeof(int));
		*k=i;
		pthread_create(&t2[i], NULL, f2,k);
	}
	pthread_join(t1, NULL);

	for (i = 0; i < nthr; i++)
	{
		pthread_join(t2[i], NULL);
	}
	printf("Suma: %d\n",sum);
	return 0;
}
