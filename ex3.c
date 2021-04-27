//Write a C program that creates two child processes A and B
//The main program will read in a continous loop single characters and after each character read
//the parent will send it to process A and B as following :
// -prcess A will receive only vowels,and will return to the parent a reuslt considerint the 
// following logic a -> @,e ->3 , i-> !,o -> 0,u -> <.The parent will display the received result
// -process B will receive onluy non-vowels,will send back to the parent character ?.The parent
// will display the received result on screen
// Parent will stop reading characters from the keyboard when "x" is typed.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc,char* argv[])
{

	int p_to_A[2],A_to_p[2],p_to_B[2],B_to_p[2];
	int respA=pipe(p_to_A);
	int resAp=pipe(A_to_p);
	int respB=pipe(p_to_B);
	int resBp=pipe(B_to_p);
		
	if( respA==-1 || resAp==-1 || respB==-1 || resBp==-1)
	{
		perror("Pipe() error: ");
		exit(EXIT_FAILURE);
	}

	//A			     
	int pidA = fork();
	if(pidA==0)
	{
		close(p_to_A[1]);
		close(A_to_p[0]);
		char c;
		while(c!='x')
		{	char cn;
			read(p_to_A[0],&c,sizeof(char));
			if(c=='a')
			{
				cn='@';
			}
			if(c=='e')
			{
				cn='3';
			}
			if(c=='i')
			{
				cn='!';
			}
			if(c=='o')
			{
				cn='0';
			}
			if(c=='u')
			{
				cn='<';
			}
			write(A_to_p[1],&cn,sizeof(char));
		}
		exit(EXIT_SUCCESS);
	}

	//B
	int pidB=fork();
	if(pidB==0)
	{
		close(p_to_B[1]);
		close(B_to_p[0]);
		char c;
		while(c!='x')
	       	{       char cn;
			read(p_to_B[0],&c,sizeof(char));
			cn='?';
			write(B_to_p[1],&cn,sizeof(char));
		}
		exit(EXIT_SUCCESS);
	}
	//parinte
	char s[5]="\0";
	char c='\0';
	while(c!='x')
	{
		printf("Dati caracterul:\n");
		scanf("%s",s);
		if(c=='x')
		{	exit(EXIT_SUCCESS);
			break;
		}
		else
		{
		c=s[0];
		char a;
		if( c=='a' || c=='e' || c=='i' || c=='o' || c=='u')
		{	//A
			write(p_to_A[1],&c,sizeof(char));
			read(A_to_p[0],&a,sizeof(char));
			printf("A: %c\n",a);
		}
		else
		{
			//B
			write(p_to_B[1],&c,sizeof(char));
			read(B_to_p[0],&a,sizeof(char));
			printf("B: %c\n",a);
			
		}
		}
	}
	 close(p_to_A[1]);
	 close(A_to_p[0]);
	 close(p_to_B[1]);
	 close(B_to_p[0]);
	 wait(0);
	 wait(0);
	 return 0;
}
