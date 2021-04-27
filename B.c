//Programul B va creea 2 procese P1 si P2 ,P1 va citi numerele din fifo-ul mentionat anterior 
//si va calcula suma acestora.Suma va fii transmisa printr-un pipe procesului P2 care va afisa pe ecran
//divizorii acesteia.Numele fifo-ului va fii dat ca argument in ambele programe
//B va sterge fifo-ul
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main(int argc,char* argv[])
{	
	char* fifo=argv[1];
	int p1_to_p2[2];
	int res=pipe(p1_to_p2);
	if(res==-1)
	{
		perror("Pipe() error: ");
		exit(EXIT_FAILURE);
	}
	if(fork()==0) //P1
	{	//inchid capatul de citire din pipe
		close(p1_to_p2[0]);
		int fd=open(fifo,O_RDONLY);
		if(fd==-1)
		{
			perror("open() error: ");
			exit(EXIT_FAILURE);
		}
		int sum=0;
		int n=1;
		while(n>0)
		{
			read(fd,&n,sizeof(int));
			sum=sum+n;
		}
		close(fd);
		unlink(fifo);
		if(0>write(p1_to_p2[1],&sum,sizeof(int)))
		{
			perror("Eroare la scrierea in P2 a sumei");
			close(p1_to_p2[1]);
		}
		close(p1_to_p2[1]);
		exit(EXIT_SUCCESS);
	}
	if(fork()==0) //P2
	{
		//inchid capatul de scriere din pipe
		close(p1_to_p2[1]);
		int sum=0;
		if(0>read(p1_to_p2[0],&sum,sizeof(int)))
		{
			perror("Eroare la citirea sumei din P1");
			close(p1_to_p2[0]);
		}
		int d=2;
		for(d=2;d<sum;d++)
		{
			if(sum%d==0)
			{
				printf("%d ",d);
			}

		}
		close(p1_to_p2[0]);
		exit(EXIT_SUCCESS);
	}
	return 0;
}
