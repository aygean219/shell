//
//Porgramul A va citi numere intregi de la tastatura si le va scrie intr-un fifo.
//Numele fifo-ului va fi dat ca argument in linia de comanda in ambele programe.
//Programul A va creea fifo-ul si programul B va sterge fifo-ul
//
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc,char* argv[])
{
	char* fifo=argv[1];
	int res=mkfifo(fifo,0666);
	if( res == -1)
	{
		perror("mkfifo error: ");
		exit(EXIT_FAILURE);
	}
	int fd=open(fifo,O_WRONLY);
	if( fd == -1)
	{
		perror("open() error: ");
		exit(EXIT_FAILURE);
	}
	int n=1;
	while(n>0)
	{
		printf("Dati un numar: ");
		scanf("%d",&n);
		write(fd,&n,sizeof(int));
	}
	close(fd);
	return 0;

}

