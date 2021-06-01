#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAX_SIZE 10

//shared variable
int counts[MAX_SIZE];


//lock obj
pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;

struct tdata
{
	int pos;
	char file[50];
};

void* do_count(void* a)
{
	struct tdata targs=*(struct tdata*)a;
	free(a);
	printf("Pos: %d File :%s\n",targs.pos,targs.file);
	int fd=open(targs.file,O_RDONLY);
	char c;
	int count=0;
	while(read(fd,&c,1)!=0)
	{
		if((c>='A')&&(c<='Z'))
		{
			count++;
		}
	}
	close(fd);
	pthread_mutex_lock(&mtx);
	counts[targs.pos]+=count;
	pthread_mutex_unlock(&mtx);
	return NULL;
}

int main(int argc,char* argv[])
{
	if(argc==1)
	{
		printf("Nu este un numar de arg bun\n");
		printf("Folosire: a.txt 3 ,b.txt 1 ..\n");
		exit(EXIT_FAILURE);
	}
	int max_thr=(int)(argc/2);
	int i=0;
	pthread_t tid[max_thr];
	for(i=0;i<max_thr;i++)
	{
		struct tdata* targ=(struct tdata*)malloc(sizeof(struct tdata));
		targ->pos=atoi(argv[2*i+2]);
		strcpy(targ->file,argv[2*i+1]);
		pthread_create(&tid[i],NULL,do_count,targ);
	}
	for(i=0;i<max_thr;i++)
	{
		pthread_join(tid[i],NULL);
	}
	printf("Counts:\n");
	for(i=0;i<MAX_SIZE;i++)
	{
		printf("counts[%d]: %d\n",i,counts[i]);
	}
	return 0;
}
