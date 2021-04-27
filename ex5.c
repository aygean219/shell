//Sa se crie un program C in care doua procese comunica prin pipe
//Procesul A:
//-va citi de la tastatura ,O SINGURA DATA,un numar natural p,pe care il va trimite prin pipe procesului B
//-va citi ,apoi,in mod repetat,cuvinte,pe care le trimite prin pipe procesului B,pana se introduce cuvantul "stop"
//Procesul B:
//-va inlocui caracterul de pe pozitia p cu "X" in fiecare cuvant primit din pipe
// si va afisa rezultatul obtinut
//sau un mesaj de eroare corespunzator
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAX_BUFFER 255
#include <string.h>
int main(int argc,char* argv[])
{
	int a_to_b[2];
	int res=pipe(a_to_b);
	if( res == -1)
	{
		perror("pipe() error: ");
		exit(EXIT_FAILURE);
	}
	int pidA=fork();
	if(pidA==0)
	{
		//A
		close(a_to_b[0]);//inchid capatul de citire din pipe
		char s1[MAX_BUFFER]="\0";
		printf("Dati numarul:\n");
		int p;
		scanf("%d",&p);
		write(a_to_b[1],&p,sizeof(int));
		while(1)
		{
			printf("Dati cuvant:\n");
			scanf("%s",s1);
			int n=0;
			n=strlen(s1);
			write(a_to_b[1],&n,sizeof(int));
			write(a_to_b[1],&s1,strlen(s1)*sizeof(char)+1);
			sleep(1);
			if(strcmp("stop",s1)==0)
			{
				break;
			}
		}
		close(a_to_b[1]);
		exit(EXIT_SUCCESS);
	}
	int pidB=fork();
	if(pidB==0)
	{	//B
		close(a_to_b[1]);
		int p;
		read(a_to_b[0],&p,sizeof(int));
		int n;
		char s[MAX_BUFFER]="\0";
		while(1)
		{
			read(a_to_b[0],&n,sizeof(int));
			read(a_to_b[0],s,n*sizeof(char)+1);
			if(strcmp("stop",s)==0)
			{
				break;
			}
			if(p<n)
			{
				s[p]='X';
				printf("Noul cuvant este --> %s\n",s);
			}
			else
			{
				printf("Nu se poate inlocui\n");
			}
			sleep(1);	
		}
		
		close(a_to_b[0]);
		exit(EXIT_SUCCESS);

	}
	int s1,s2;
	wait(&s1);
	wait(&s2);
	return 0;
}
