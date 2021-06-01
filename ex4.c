/*
 * Write a program that creates N threads(N command line argument),
 * passing as an argument to each thread a number and a string that
 * are read from keyboard.Thread sleep for 2 seconds,then the y add
 * the number to one of global lists of numbers multiples of 7 or not
 * while for each string they store it in a global aray of strings
 * The main thread prints at the end the values of the global variables
 * Ensure that no more than 4 threads are runing at the same time.
 * The threads must be dynamically allocated in memory.
 * Ensure there are no memory leaks.Use efficient synchronization machanisms.
 */
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <semaphore.h>

#define TIME_SLEEP 2
#define N_THR 4
#define MAX_NR 1000
#define LEN 20

pthread_mutex_t mtx[2]={PTHREAD_MUTEX_INITIALIZER,PTHREAD_MUTEX_INITIALIZER};

sem_t sem;
pthread_t* t;

int m7[MAX_NR];
int Nm7[MAX_NR];
int nr_m7=0;
int nr_Nm7=0;
char** strings;
int nr_strings=0;

struct tdata
{
	int number;
	char* str;
};

void* funct(void* a)
{
	struct tdata k=*(struct tdata*)a;
	free(a);
	sem_wait(&sem);
	sleep(TIME_SLEEP);
	pthread_mutex_lock(&mtx[0]);
	if(k.number%7==0)
	{
		m7[nr_m7]=k.number;
		nr_m7++;
	}
	else
	{
		Nm7[nr_Nm7]=k.number;
		nr_Nm7++;
	}
	pthread_mutex_unlock(&mtx[0]);
	pthread_mutex_lock(&mtx[1]);
	strings[nr_strings]=malloc(strlen(k.str)*sizeof(char)+1);
	strcpy(strings[nr_strings],k.str);
	nr_strings++;
	pthread_mutex_unlock(&mtx[1]);
	sem_post(&sem);
	return NULL;
}

int main(int argc,char* argv[])
{
	int nthr=atoi(argv[1]);
	t=(pthread_t*)malloc(nthr*sizeof(pthread_t));
	strings=malloc(nthr*sizeof(char*));
	int i=0;
	sem_init(&sem,0,N_THR);
	for(i=0;i<nthr;i++)
	{
		struct tdata* citire=malloc(sizeof(struct tdata));
		citire->str=malloc(LEN*sizeof(char));
		printf("Number:\n");
		scanf("%d",&citire->number);
		printf("String:\n");
		scanf("%s",citire->str);
		pthread_create(&t[i],NULL,funct,citire);
	}
	for(i=0;i<nthr;i++)
	{
		pthread_join(t[i],NULL);
	}
	printf("\nList of numbers multiples of 7:\n");
	for(i=0;i<nr_m7;i++)
	{
		printf("%d ",m7[i]);
	}
	printf("\nList of numbers not multiples of 7:\n");
	for(i=0;i<nr_Nm7;i++)
       	{
		printf("%d ",Nm7[i]);
	}
	printf("\nStrings:\n");
	for(i=0;i<nr_strings;i++)
	{
		printf("%s ",strings[i]);
	}
	for(i=0;i<nr_strings;i++)
	{
		free(strings[i]);
	}
	free(strings);
	free(t);
	sem_destroy(&sem);
	return 0;
}
