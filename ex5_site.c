//4. Să se scrie un program C care creează un proces copil cu care comunică prin pipe.
// Procesul părinte citeşte de la tastatură două numere întregi pe care le trimite 
// prin pipe procesului copil, iar procesul copil returnează prin pipe suma lor.
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
	int p_to_c[2],c_to_p[2];
	int res=pipe(p_to_c);
	int res2=pipe(c_to_p);
	int nr1=0,nr2=0,sum=0;
	if(res ==-1 || res2 ==-1)
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
	{	//inchid capatul de scriere din pipe parinte->copil
		close(p_to_c[1]);
		//inchid capatul de citire din pipe copil->parinte
		close(c_to_p[0]);
		int nr1=0,nr2=0,sum=0;
		//citesc in copil primul numar
		if(0 > read(p_to_c[0], &nr1, sizeof(int))) {
                    perror("Error:la citirea numarului 1 in copil");                              		close(p_to_c[0]);
		 }
                //citesc in copil numarul 
                if(0 > read(p_to_c[0], &nr2,sizeof(int))) {
                    perror("Error:la citirea numarului 2 in copil");
		    close(p_to_c[0]);}
		sum=nr1+nr2;
		if(0>write(c_to_p[1],&sum,sizeof(int)))
		{
			perror("Eroare la scrierea in copil a sumei");
			close(c_to_p[1]);
		}
		close(c_to_p[1]);	
		close(p_to_c[0]);
		exit(EXIT_SUCCESS);

	}
	//in procesul parinte
	//inchid capatul de citire din pipe parinte->copil
	close(p_to_c[0]);
	//inchid capatul de scriere din pipe copil->parinte
	close(c_to_p[1]);
	FILE* fis;
	fis=fopen(argv[1],"r");	
	fscanf(fis,"%d",&nr1);
	fscanf(fis,"%d",&nr2);
	//scriu in copil primul numar
	if(0 > write(p_to_c[1], &nr1, sizeof(int))) {
            perror("Error:la scrierea in copil a caracterului");
            close(p_to_c[1]);
        }
	//scriu in copil numarul 2
	if(0 > write(p_to_c[1], &nr2,sizeof(int))) {
            perror("Error:la scrierea in copil a  sirului de caractere");
            close(p_to_c[1]);
        }
	if(0>read(c_to_p[0],&sum,sizeof(int)))
	{
		perror("Error:la citirea sumei in parinte");
		close(c_to_p[0]);
	}
	printf("%d + %d = %d\n",nr1,nr2,sum);
	close(c_to_p[0]);
	close(p_to_c[1]);
	int status;
	wait(&status);
	//inchid capatul de scriere din pipe
	//close(p_to_c[1]);
	return 0;
}	
