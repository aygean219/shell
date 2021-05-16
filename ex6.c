//
// lock_4.c - Using a semaphore
//
// sem_init()
// sem_destroy()
//
// sem_wait()
// sem_post()
//


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>

#define MAX_READ  1000   // maximum number of readers
#define MAX_WRITE 100    // maximum number of writers

int seats = 100;        // initial number of seats
float price = 75.0f;    // initial price per seat

// declare a semaphore
sem_t sem;

//
// Readers start routine
//
void* check(void* a)
{
	int k = *(int*)a;

	sem_wait(&sem);		// lock

	printf("Reader %2d: Locuri %d, pret %f\n",
		k, seats, price * (2 - seats/100.0f));

	sem_post(&sem);		// unlock
	free(a);
	return NULL;
}

//
// Writers start routine
//
void* buy(void* a)
{
	int k = *(int*)a;

	sem_wait(&sem);		// lock

	printf("Writer %2d: Locul meu este %d, pret %f\n",
		k, seats, price * (2 - seats/100.0f));
	seats--;

	sem_post(&sem);         // unlock
	free(a);
	return NULL;
}



int main(int argc, char* argv[])
{
	// create the semaphore
	sem_init(&sem, 0, 1);

	int i;
	pthread_t tr[MAX_READ];
	
	for (i = 0; i < MAX_READ; i++)
	{
		int *tid = (int*)malloc(1 * sizeof(int));
		*tid = i;
		pthread_create(&tr[i], NULL, check,tid);	// create readers
	}

	pthread_t tw[MAX_WRITE];
	for (i = 0; i < MAX_WRITE; i++)
	{
		int *tid = (int*)malloc(1 * sizeof(int));
		*tid = i;
		pthread_create(&tw[i], NULL, buy, tid);	// create writers
	}

	for (i = 0; i < MAX_READ; i++)
	{
		pthread_join(tr[i], NULL);	// wait for readers to finish
	}

	for (i = 0; i < MAX_WRITE; i++)
	{
		pthread_join(tw[i], NULL);      // wait for writers to finish
	}

	// destroy the semaphore
	sem_destroy(&sem);
	
	return 0;
}
