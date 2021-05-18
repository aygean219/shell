//Write a program that reads from keyboard N integers and store them into a vector 
//(N is constant you can hardcode in your program )
//After all integers are read,there will be created N threads that will receive one of the integer read
//and it will check if ends with 5 or is dividing with 2,increasing two global variables each time
//the number ended with 5 respectively divided with 2
//The main program will print at the end these two counters and will finish its execution

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

void *func(void *arg)
{
	int x=*((int*) arg);
	if(x%10==5)
	{
		pthread_mutex_lock(&mutexes[0]);
		v5++;
		pthread_mutex_unlock(&mutexes[0]);
	}
	if(x%2==0)
	{
		pthread_mutex_lock(&mutexes[1]);
		v2++;
		pthread_mutex_unlock(&mutexes[1]);
	}
	free(arg);
	return NULL;
}
int main(int argc,char* argv[])
{

	int N=0;
	printf("N: ");
	scanf("%d",&N);
	printf("\nDati numerele: \n");
	vect=malloc(sizeof(int)*N);
	int i=0;
	for(i=0;i<N;i++)
	{
		scanf("%d",&vect[i]);
	}
	mutexes=malloc(sizeof(pthread_mutex_t)*2);
	for(i=0;i<2;i++)
	{
		if(0>pthread_mutex_init(&mutexes[i],NULL))
		{
			perror("Error on creating mutex\n");
			exit(1);
		}
	}
	pthread_t *thrds=malloc(sizeof(pthread_t)*N);
	for(i=0;i<N;i++)
	{
		int *k=(int*)malloc(sizeof(int));
		*k=vect[i];
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
	printf("V2: %d\n",v2);
	printf("V5: %d\n",v5);
	for(i=0;i<2;i++)
	{
		pthread_mutex_destroy(&mutexes[i]);
	}
	free(thrds);
	free(mutexes);
	free(vect);
	return 0;
}


