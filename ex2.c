//
// ex_model.c
//
//2. Citiți 100.000 de numere dintr-un fișier binar cu numere aleatoare și determinați
//mai apoi, utilizând 100 thread-uri, câte numere sunt divizibile cu 5.
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

// maximum number of threads
#define MAX_THR 100
#define MAX_NR 100000
#define MAX_LIM 1000
// shared variable
int n_div=0;
int nr[MAX_NR];

// lock object
pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;

// thread start routine
void* do_count(void* a)
{
	int k = *(int*)a;
	//printf("Eu sunt thread-ul %d\n", k);
	int min=k*MAX_LIM;
	int max=min+MAX_LIM;
	int i;
	for(i=min;i<max;i++)
	{
		pthread_mutex_lock(&mtx);
		if(nr[i]%5==0)
		{
			n_div++;
		}
		pthread_mutex_unlock(&mtx);
	}
	free(a);
	return NULL;
}



int main(int argc, char* argv[])
{
	pthread_t t[MAX_THR];
	
	int i;
	int fd=open("random-file.bin",O_RDONLY);
	for(i=0;i<MAX_NR;i++)
	{
		read(fd,&nr[i],1);
		
	}
	close(fd);
	for (i = 0; i < MAX_THR; i++)
	{
		int* k=(int*)malloc(sizeof(int));
		*k=i;
		pthread_create(&t[i], NULL, do_count, k);
	}

	for (i = 0; i < MAX_THR; i++)
	{
		pthread_join(t[i], NULL);
	}
	printf("Numarul de divizori :%d\n",n_div);
	int n_div2=0;
	for(i=0;i<MAX_NR;i++)
	{
		if(nr[i]%5==0)
		{
			n_div2++;
		}
	}
	printf("Numar divizori 2: %d\n",n_div2);
	return 0;
}
