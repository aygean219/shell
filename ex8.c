#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include <time.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <math.h>
#include <limits.h>

#define RANGE 200

int* numbers;
int N = 0;
int fileD;
int globalMin = INT_MAX;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier;

void* threadRoutine(void* idPtr)
{
    int id = *(int*)idPtr;
    int start = id * RANGE;
    int end = (id + 1) * RANGE;

    int i;

    for (i = start; i < end; i++)
        read(fileD, &numbers[i], 1);

    int M = rand() % 256;
    int absDiff = INT_MAX;
    int posClosest = 0;

    for (i = start; i < end; i++)
        if (numbers[i] % 5 == 0 && abs(numbers[i] - M) < absDiff)
        {
            absDiff = abs(numbers[i] - M);
            posClosest = i;
        }

    printf("Thread %d, range %d->%d, M = %d, closest number = %d\n", id, start, end, M, numbers[posClosest]);

    pthread_barrier_wait(&barrier);

    pthread_mutex_lock(&mutex);

    if (globalMin > numbers[posClosest])
        globalMin = numbers[posClosest];

    pthread_mutex_unlock(&mutex);

    free(idPtr);
    return NULL;
}

int main()
{
    printf("N=");
    scanf("%d", &N);

    numbers = malloc(sizeof(int) * N);

    int thrNum = N / RANGE;
    pthread_t* threads = malloc(sizeof(pthread_t) * thrNum);

    pthread_barrier_init(&barrier, NULL, thrNum);

    fileD = open("random-file", O_RDONLY);
    if (fileD == -1)
    {
        perror("error open()\n");
        exit(1);
    }

    srand(time(NULL));

    int i;
    for (i = 0; i < thrNum; i++)
    {
        int* idPtr = malloc(sizeof(int));
        *idPtr = i;
        pthread_create(&threads[i], NULL, threadRoutine, idPtr);
    }

    for (i = 0; i < thrNum; i++)
        pthread_join(threads[i], NULL);

    printf("Global min = %d\n", globalMin);
    free(numbers);
    free(threads);
    pthread_barrier_destroy(&barrier);
    return 0;
}
