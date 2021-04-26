// 3. Să se scrie un program C care creează un proces copil cu care comunică prin pipe.
// Procesul părinte citeşte de la tastatură un caracter c şi un şir s şi le trimite prin pipe
//  procesului copil, iar procesul copil verifică şi afişează numărul de apariţii
//   ale caracterului c în şirul s
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include <string.h>
#define BUF_SIZE 255
int main(int argc,char* argv[])
{
	//crez un pipe
	int p_to_c[2];
	int res=pipe(p_to_c);
	char s1[10]="\0";
	char s2[BUF_SIZE]="\0";
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
		close(p_to_c[1]);
		char c='\0';
		char cs='\0';
		//printf("-----1");
		if(0 > read(p_to_c[0], &c, sizeof(char))) 
		{
            		perror("Error:la citirea din copil a caracterului");
            		close(p_to_c[0]);
        	}
		//citirea sirului de caractere
		int i=0;
		//printf("-----2");
		while(read(p_to_c[0],&cs,sizeof(char))>0)
		{	//printf("%c",cs);
			if(c==cs)
			{
				i++;
			}
		}
		printf("Caracterul %c apare de %d\n",c,i);
		close(p_to_c[0]);
		exit(EXIT_SUCCESS);

	}
	//in procesul parinte
	//inchid capatul de citire din pipe
	close(p_to_c[0]);
	printf("Dati caracterul al carui numar de aparitii doriti sa-l vedeti :\n");
	scanf("%s",s1);
	printf("Dati sirul de caractere in cautati :\n");
	//scanf(" %s",s2);
	read(0,s2,BUF_SIZE*sizeof(char));
	char c=s1[0];
	//scriu in copil caracterul
	if(0 > write(p_to_c[1], &c, sizeof(char))) {
            perror("Error:la scrierea in copil a caracterului");
            close(p_to_c[1]);
        }
	//scriu sirul de caractere
	if(0 > write(p_to_c[1], s2, strlen(s2)*sizeof(char))) {
            perror("Error:la scrierea in copil a  sirului de caractere");
            close(p_to_c[1]);
        }
	close(p_to_c[1]);
	int status;
	wait(&status);
	//inchid capatul de scriere din pipe
	//close(p_to_c[1]);
	return 0;
}	
