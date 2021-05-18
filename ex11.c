//3. Write a program that creates 20 threads, giving each thread a string as parameter.
// Each thread will count and add to the global variables v and n as follows: 
// the number of vowels contained by the string added to v, 
// and the number of digits contained in the string added to n.
//  Synchronise threads using mutex and check for memory leaks.
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_THR 20

int v=0,n=0;
pthread_mutex_t mutex[2];

void *func(void *arg)
{
	char* str=((char*) arg);
	int len=strlen(str);
	for(int i=0;i<len;i++)
	{
		if(str[i]=='a' || str[i]=='e' || str[i]=='o' || str[i]=='i' || str[i]=='u')
		{
			pthread_mutex_lock(&mutex[0]);
			v++;
			pthread_mutex_unlock(&mutex[0]);
		}
		if(str[i]>='0' && str[i]<='9')
		{
			pthread_mutex_lock(&mutex[1]);
      			n++;
			pthread_mutex_unlock(&mutex[1]);
		}
	}
	return NULL;
}

int main(int argc,char* argv[])
{
	if(argc<2)
	{
		printf("Please provide at least one argument\n");
		exit(1);
	}

	pthread_t *thrds=malloc(sizeof(pthread_t)*(argc-1));
	int i=0;
	//pthread_mutex_t *mutex=malloc(sizeof(pthread_mutex_t)*2);
	for(i=0;i<2;i++)
	{
		if(0>pthread_mutex_init(&mutex[i],NULL))
		{
			perror("Error on creating mutexes\n");
			exit(1);
		}
	}
	for(i=0;i<argc-1;i++)
	{
		if(0>pthread_create(&thrds[i],NULL,func,argv[i+1]))
		{
			perror("Error on create thread");
		}
	}
	for(i=0;i<argc-1;i++)
	{
		if(0>pthread_join(thrds[i],NULL))
		{
			perror("Error waiting for thread");
		}
	}
	printf("v:  %d\n",v);
	printf("n:  %d\n",n);
	for(i=0;i<2;i++)
	{
		pthread_mutex_destroy(&mutex[i]);
	}
	free(thrds);
//	free(mutex);
	return 0;
}
