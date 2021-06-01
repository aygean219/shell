#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_THR 10
#define MAX_NR 1000

int nr_arr[MAX_NR];
int max_arr[MAX_THR];

pthread_barrier_t barr;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void* funct(void* a)
{

	int k=*(int*)a;
	int min=k*(MAX_NR/MAX_THR);
	int max=min+(MAX_NR/MAX_THR);
	int i=0;
	int max_loc=nr_arr[min];
	for(i=min;i<max;i++)
	{
		if(nr_arr[i]>max_loc)
			max_loc=nr_arr[i];
	}
	max_arr[k]=max_loc;
	pthread_barrier_wait(&barr);
	int dif_max; 
	for(i=0;i<MAX_THR;i++)
	{
		int diff=abs(max_arr[i]-max_loc);
		if(diff>dif_max)
			dif_max=diff;
	}
	printf("Thread %d maximul pe thread:%d\n",k,max_loc);
	printf("Thread %d diferenta maxima: %d\n",k,dif_max);
	return NULL;
}

int main(int argc, char* argv[])
{
	pthread_t thr[MAX_THR];

	pthread_barrier_init(&barr, NULL, MAX_THR);	// init the barrier

	int i;

	int bnf=open("random-file",O_RDONLY);
	for(i=0;i<MAX_NR;i++)
	{
		read(bnf,&nr_arr[i],1);
	}
	close(bnf);
	for (i = 0; i < MAX_THR; i++)	
	{
		int *tid = (int*)malloc(1 * sizeof(int));
		*tid = i;
		 pthread_create(&thr[i], NULL, funct, tid);
	}
	for (i = 0; i < MAX_THR; i++)
	{
		pthread_join(thr[i], NULL);
	}

	pthread_barrier_destroy(&barr);

	return 0;
}
