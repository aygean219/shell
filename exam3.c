//aceeasi paritate
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int *vect;
pthread_mutex_t *mutexes;
int v2=0,v5=0;
int j=0;

void *func(void *arg)
{
	int x=*((int*) arg);
	int nr1=random()%999+0;
	int nr2=random()%999+0;
	printf("Thread: %d ,Number1: %d,Number2: %d\n",x,nr1,nr2);
	if(nr1%2==nr2%2)
	{
		pthread_mutex_lock(&mutexes[0]);
		vect[j]=nr1;
		j++;
		vect[j]=nr2;
		j++;
		pthread_mutex_unlock(&mutexes[0]);
	}
	free(arg);
	return NULL;
}
int main(int argc,char* argv[])
{
	srandom(time(0));
	int N=atoi(argv[1]);
	vect=malloc(sizeof(int)*N*2);
	int i=0;
	for(i=0;i<N;i++)
	{
		vect[i]=0;
	}
	mutexes=malloc(sizeof(pthread_mutex_t));
	if(0>pthread_mutex_init(&mutexes[0],NULL))
		{
			perror("Error on creating mutex\n");
			exit(1);
		}
	
	pthread_t *thrds=malloc(sizeof(pthread_t)*N);
	for(i=0;i<N;i++)
	{
		int *k=(int*)malloc(sizeof(int));
		*k=i;
		if(0>pthread_create(&thrds[i],NULL,func,k))
		{
			perror("Error on creating threads");	
		}
	}
	for(i=0;i<N;i++)
	{
		if(0>pthread_join(thrds[i],NULL))
		{
			perror("Error wainting for thread");
		}
	}
	for(i=0;i<j;i++)
	{
		printf("%d ",vect[i]);
	}
	pthread_mutex_destroy(&mutexes[0]);
	free(thrds);
	free(mutexes);
	free(vect);
	return 0;
}


