//
// barrier_1.c - Using barriers
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_THR 10		// number of threads
#define MAX_NR 100		// numbers of integers in the array

int nr_arr[MAX_NR];
int max_arr[MAX_THR];


pthread_barrier_t barr;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;


//
// List array
//
void* func(void* a)
{
	int k=*(int*)a;
	int min=k*(MAX_NR/MAX_THR);
	int max=min+(MAX_NR/MAX_THR);
	int i;
	int max_loc=nr_arr[min];
	for(i=min+1;i<max;i++)
	{
		if(nr_arr[i]>max_loc)
		{
			max_loc=nr_arr[i];
		}
	}
	max_arr[k]=max_loc;
	pthread_barrier_wait(&barr);
	int dif_max=0;
	for(i=0;i<MAX_THR;i++)
	{
		int diff=abs(max_arr[i]-max_loc);
		if(diff>dif_max)
		{
			dif_max=diff;
		}
	}
	printf("Thread %d maximul pe thread %d\n",k,max_loc);
	printf("Thread %d diferenta maxima: %d\n",k,dif_max);
	free(a);
	return NULL;
}
//
// Thread routine
//
int main(int argc, char* argv[])
{
	pthread_t thr[MAX_THR];

	pthread_barrier_init(&barr, NULL, MAX_THR);// init the barrier

	int i;

	int bnf=open("random-file.bin",O_RDONLY);
	for(i=0;i<MAX_NR;i++)
	{
		read(bnf,&nr_arr[i],1);
	}

	for (i = 0; i < MAX_THR; i++)// create the threads
	{
		int* tid=(int*)malloc(sizeof(int));
		*tid = i;
		pthread_create(&thr[i], NULL, func, tid);
	}

	for (i = 0; i < MAX_THR; i++)
	{
		pthread_join(thr[i], NULL);// wait for each thread to finish
	}
	
	pthread_barrier_destroy(&barr);// destroy the barrier

	return 0;
}
