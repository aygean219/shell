//2. Write a program that creates 4 threads and had 3 global variables v5, v2, v3.
//Each thread generates a random number and:
//- if the number is multiple of 2 increments v2
//- if the number is multiple of 3, increments v3
//- if the number is multiple of 5 increments v5
//
//The number can be a multiple of more numbers (ex. for 10 we will increment both V2 and V5)
//
//Threads print the generated numbers and stop when 30 numbers have been generated.
//
//The main program prints the 3 global variables.
//!!! Use mutex for synchronisation.
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define MAX_THR 4

int v5=0,v2=0,v3;
int ind=0;
pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;

void* functie(void* arg)
{
	//int k=*(int*)arg;
	//free(arg);
	while(1)
	{
		if(ind>=30) break;
		pthread_mutex_lock(&mtx);
		if(ind==30)
		{
			pthread_mutex_unlock(&mtx);
			break;
		}
		int nr=rand()%20+1;
		printf("Index --> %d Numar --> %d\n",ind,nr);
		ind++;
		if(nr%2==0)
		{
			v2++;
		}
		if(nr%3==0)
		{
			v3++;
		}
		if(nr%5==0)
		{
			v5++;
		}
		pthread_mutex_unlock(&mtx);
	}
	return NULL;

}
int main(int argc,char* argv[])
{

	pthread_t t[MAX_THR];
	srand(time(0));
	int* k=(int*)malloc(sizeof(int));
	int i=0;
	for(i=0;i<MAX_THR;i++)
	{
		*k=i;
		pthread_create(&t[i],NULL,functie,k);
	}
	for(i=0;i<MAX_THR;i++)
	{
		pthread_join(t[i],NULL);
	}
	free(k);
	printf("v2 :%d\n",v2);
	printf("v3 :%d\n",v3);
	printf("v5 :%d\n",v5);
	return 0;
}
