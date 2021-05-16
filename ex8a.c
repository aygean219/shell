//Scrieti un program C care citeste o matrice de intregi dintr-un fisier.
// Programul creeaza un numar de thread-uri egal cu numarul de randuri in matrice, 
// iar fiecare thread calculeaza suma numerelor de pe un rand.
//  Procesul principal asteapta ca thread-urile sa isi incheie executia si apoi afiseaza sumele.
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct
{
	int **m;
	int *linii,*coloane;
	pthread_mutex_t mutex;
	int *suma;
}data;

void *func(void *arg)
{
	data dt=*((data*)arg);
	int lin=(*dt.linii);
	int col=(*dt.coloane);
	int i;
	//int sum=0;
	//pthread_mutex_lock(&dt.mutex);
	int sum=0;
	for(i=0;i<col;i++)
	{
		//pthread_mutex_lock(&dt.mutex);
		sum+=dt.m[lin][i];
		//pthread_mutex_unlock(&dt.mutex);
	}
	if(sum>0)
	{
		pthread_mutex_lock(&dt.mutex);
		dt.suma[lin]=sum;
		pthread_mutex_unlock(&dt.mutex);
	}
	return NULL;
}
int main(int argc,char* argv[])
{
	FILE* fd=fopen("matrice.txt","r");
	int *linii=malloc(sizeof(int));
	int *coloane=malloc(sizeof(int));
	fscanf(fd,"%d",linii);
	fscanf(fd,"%d",coloane);
	int **m=(int**)malloc((*linii)*sizeof(int*));
	int i=0,j=0;
	int *suma=(int*)malloc((*linii)*sizeof(int));
	for(i=0;i<(*linii);i++)
	{
		suma[i]=0;
	}
	for(i=0;i<(*linii);i++)
	{
		m[i]=(int*)malloc((*coloane)*sizeof(int));
	}
	for(i=0;i<(*linii);i++)
	{
		for(j=0;j<(*coloane);j++)
		{
			fscanf(fd,"%d",&m[i][j]);
		}
	}
	pthread_t *thrds=malloc(sizeof(pthread_t)*(*linii));
	pthread_mutex_t mutex;//=PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_init(&mutex, NULL);
	data *args=malloc(sizeof(data)*(*linii));
	int *nr=malloc(sizeof(int));
	for(i=0;i<(*linii);i++)
	{
		args[i].mutex=mutex;
		*nr=i;
		args[i].suma=suma;
		args[i].linii=nr;
		args[i].coloane=coloane;
		args[i].m=m;
		if(0>pthread_create(&thrds[i],NULL,func,(void*)&args[i]))
		{
			perror("Error on create thread");
		}
	}
	for(i=0;i<(*linii);i++)
	{
		if(0>pthread_join(thrds[i],NULL))
		{
			perror("Error waiting for thread");
		}
	}
	for(i=0;i<(*linii);i++)
	{
		printf("Suma la linia %d este %d\n",i,suma[i]);
	}
	for(i=0;i<(*linii);i++)
	{
		free(m[i]);
	}
	free(m);
	pthread_mutex_destroy(&mutex);
	free(suma);
	free(linii);
	free(coloane);
	free(thrds);
	free(args);
	fclose(fd);
	//free(fd);
	free(nr);
	return 0;
}

