//2. Să se scrie un program C care creează un proces copil cu care comunică prin pipe.
// Procesul părinte citeşte de la tastatură un număr natural şi îl trimite prin pipe
//  procesului copil, iar procesul copil verifică şi afişează dacă acest număr este prim.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
int main(int ardc,char* argv[])
{
	//crez un pipe
	int pd[2];
	int res=pipe(pd);
	if(res ==-1)
	{
		perror("Pipe() error");
		exit(EXIT_FAILURE);
	}
	int pid=fork();
	if(pid == -1)
	{
		perror("fork() ERROR");
		exit(EXIT_FAILURE);
	}

	if(pid == 0)
	{	//inchid capatul de scriere din pipe
		close(pd[1]);
		int n=1;
		while(n>0)
		{
			read(pd[0],&n,sizeof(int));
			if(n%2==0 || n<=1)
			{
				printf("IN CHILD --->Numarul citit %d NU este prim\n",n);
			}
			else
			{	int ok=0;
				int i=3;
				for(i=3;i<n&&ok==0;i++)
				{
					if(n%i==0)
					{	ok=1;
						printf("IN CHILD --->Numarul citit %d NU este prim\n",n);
					}
				}
				if(ok==0)
				{
					printf("IN CHILD --->Numarul citit %d ESTE prim\n",n);
				}
		
			}
		}
		close(pd[0]);
		exit(EXIT_SUCCESS);

	}
	//in procesul parinte
	//inchid capatul de citire din pipe
	close(pd[0]);
	int n=1;
	while(n>0)
	{
		printf("Dati numarul : ");
		scanf("%d",&n);
		write(pd[1],&n,sizeof(int));
		sleep(1);
	}
	int status;
	wait(&status);
	//inchid capatul de scriere din pipe
	close(pd[0]);
	return 0;
}	
