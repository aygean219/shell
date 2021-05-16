//4. A C program receives command line args pairs of numbers,
// and creates for each pair a thread that checks is the two numbers 
// are relatively prime (gcd=1), incrementing a global variable.
//  The program prints at the end how many relatively prime pairs 
//  have been found and the respective pairs.
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

int v=0;
pthread_mutex_t mutex;

typedef struct 
{
	int x,y;
}pair;


int gcd(int a,int b)
{
	if(a==0) return b;
	if(b==0) return a;
	if(a==b) return a;
	if(a>b)
		return gcd(a-b,b);
	return gcd(a,b-a);
}

void *functie(void *arg)
{
	pair p=*((pair*)arg);
	pthread_mutex_lock(&mutex);
	if(gcd((p.x),(p.y))==1)
	{
		v++;
		printf("x: %d  y: %d\n",p.x,p.y);
	}
	pthread_mutex_unlock(&mutex);
	free(arg);
	return NULL;
}

int main(int argc,char* argv[])
{
	if(argc<3)
	{
		printf("Please provide at least one argument\n");
		exit(1);
	}
	pthread_t *thrds=malloc(sizeof(pthread_t)*(argc-1));
	if(0>pthread_mutex_init(&mutex,NULL))
	{
		perror("Error on creating mutex\n");
		exit(1);
	}
	int i=0;
	for(i=1;i<argc-1;i++)
	{
		pair *k=(pair*)malloc(sizeof(pair));
		k->x=atoi(argv[i]);
		k->y=atoi(argv[i+1]);
		if(0>pthread_create(&thrds[i],NULL,functie,k))
		{
			perror("Error on create thread\n");
		}
	}

	for(i=1;i<argc-1;i++)
	{
		pthread_join(thrds[i],NULL);
	}
	pthread_mutex_destroy(&mutex);
	printf("v= %d\n",v);
	free(thrds);
	return 0;
}













