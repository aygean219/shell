//
// lock_3.c - Using read-write lock object
//
// pthread_rwlock_init()    - initialize a read-write lock object
// pthread_rwlock_destroy() - destroy the read-write lock object
//
// pthread_rwlock_rdlock()
// pthread_rwlock_wrlock()
// pthread_rwlock_unlock()
//


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define MAX_READ  1000   // maximum number of readers
#define MAX_WRITE 100    // maximum number of writers

int seats = 100;        // initial number of seats
float price = 75.0f;    // initial price per seat

//declare a read-write lock object
pthread_rwlock_t rwl;

//
// Readers start routine
//
void* check(void* a)
{
	int k = *(int*)a;

	pthread_rwlock_rdlock(&rwl);      // lock for read

	printf("Reader %2d: Locuri %d, pret %f\n",
		k, seats, price * (2 - seats/100.0f));

	pthread_rwlock_unlock(&rwl);      // unlock
	free(a);
	return NULL;
}

//
// Writers start routine
//
void* buy(void* a)
{
	int k = *(int*)a;

	pthread_rwlock_wrlock(&rwl);      // lock for write

	printf("Writer %2d: Locul meu este %d, pret %f\n",
		k, seats, price * (2 - seats/100.0f));
	seats--;

	pthread_rwlock_unlock(&rwl);      // unlock
	free(a);
	return NULL;
}



int main(int argc, char* argv[])
{
	// create the read-write lock object
	pthread_rwlock_init(&rwl, NULL);

	int i;
	pthread_t tr[MAX_READ];
	
	for (i = 0; i < MAX_READ; i++)
	{
		 int *tid = (int*)malloc(1 * sizeof(int));
		 *tid = i;
		pthread_create(&tr[i], NULL, check, tid);	// create readers
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

	// destroy the read-write lock object
	pthread_rwlock_destroy(&rwl);

	return 0;
}
