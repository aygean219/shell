/*
 *  * Sa se scrie un program C care:
 * 1.Va citi de la tastatura un numar natural N={3000,4000,5000,6000}
 * 2.Va crea un singur thread care va genera:
 * - N numere aleatoare (pe 1 OCTET)din fisierul binar "/tmp/test-file" 
 *   si le va stoca intr-o variabila globala;
 * -va genera un numar aleator M (0 < M < 256)
 * 3.Va crea,apoi,un numar potrivit de thread-uri care vor procesa numerele
 * care au fost citite din fisier in primul thread creat si care:
 * -vor determina si afisa numarul de aparitii a numarului aleator M in fiecare 
 *  interval de 200 de numere intregi consecutive;
 *  -vor calcula,la final,si vor stoca intr-o variabila globala comuna,
 *  numarul total de aparitii ale numarului aleator M
 *
 */
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

// maximum number of threads
#define MAX_SIZE 6000
#define MAX_NUM 200
#define MAX_NR 30
// shared variable
int numbers[MAX_SIZE];
int full=0;
int nr=0;
// lock object
pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv=PTHREAD_COND_INITIALIZER;
int M;
// thread start routine
void* f1(void* a)
{
	int N = *(int*)a;
	int i=0;
	free(a);
	sleep(5);

	srand(time(NULL));
	int fd=open("random-file",O_RDONLY);

	for (i = 0; i < N; i++)
	{
		pthread_mutex_lock(&mtx);
		while(full==1)
		{
			pthread_cond_wait(&cv,&mtx);
		}
		read(fd,&numbers[i],1);
		if(i==N-1)
		{
			M=rand()%255+1;
		}
		pthread_cond_signal(&cv);
		pthread_mutex_unlock(&mtx);
	}

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
	int nr_interval=0;
	for (i = min; i < max; i++)
	{
		pthread_mutex_lock(&mtx);
		while(full==0)
		{
			pthread_cond_wait(&cv,&mtx);
		}
		if(numbers[i]==M)
		{
			nr_interval++;
		}
		if(i==max-1)
		{
			nr=nr+nr_interval;
			printf("Threadul : %d, Numar aparitii pe interval: %d\n",k,nr_interval);
		}		
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
	int i;
	//calculez numarul de thread-uri de tip consumator
	int nthr=(int)(N/MAX_NUM);

	pthread_t t2[nthr];

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
	printf("Numar total de aparitii: %d\n",nr);
	return 0;
}
