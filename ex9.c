//Write a C program that receives integers as command line argument. 
//The program will keep a frequency vector for all digits. 
//The program will create a thread for each argument that counts 
//the number of occurences of each digit and adds the result 
//to the frequency vector. Use efficient synchronization.
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
int vect[10];
pthread_mutex_t mutexes[10];


void *func(void *arg)
{
	int x=*((int*)arg);
	while(1)
	{	
		pthread_mutex_lock(&mutexes[x%10]);
		int n=x%10;
		if(x==0) 
		{	
			pthread_mutex_unlock(&mutexes[n]);
			break;
		}
		int nn=n;
		x=x/10;
		vect[nn]++;
		pthread_mutex_unlock(&mutexes[nn]);
	}
	free(arg);
	return NULL;
}

int main(int argc,char *argv[])
{
	if(argc<2)
	{
		printf("Please provice at least one argument.\n");
		exit(1);
	}
	pthread_t *thrds=malloc(sizeof(pthread_t)*(argc-1));
	int i;
	for(i=0;i<10;i++)
	{
		if( 0>pthread_mutex_init(&mutexes[i],NULL))
		{
			perror("Error on creating mutexes\n");
			exit(1);
		}
	}
	for(i=0;i<=9;i++)
	{
		vect[i]=0;
	}
	for( i=0;i<argc-1;i++)
	{
		int *k=(int*)malloc(sizeof(int));
		*k=atoi(argv[i+1]);
		if( 0>pthread_create(&thrds[i],NULL,func,k))
		{
			perror("Error on create thread");
		}
	}
	for( i=0;i<argc-1;i++)
	{
		if(0>pthread_join(thrds[i],NULL))
		{
			perror("Error waiting for thread");
		}
	}
	printf("Total digits \n 0: %d\n 1: %d\n 2: %d\n 3: %d\n 4: %d\n 5: %d\n 6: %d\n 7: %d\n 8: %d\n 9: %d\n",vect[0],vect[1],vect[2],vect[3],vect[4],vect[5],vect[6],vect[7],vect[8],vect[9]);
	for(i=0;i<10;i++)
	{
		pthread_mutex_destroy(&mutexes[i]);
	}
	free(thrds);
	return 0;
}	
