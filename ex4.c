//Write a C program that creates 2 child processes A and B:
//The main program will read in continous loop numbers and after each number read,the parent will send it to process
//A aand B as following:
//-process A will receive even numbers,will add one to the received numbers and will
//send them back to the parent that will display the number on the screen
//-process B will receive odd numbers,will send them back to the parent the last digit of the number
//that will display this last digit on screen
// Parent will stop reading numbers from the keyboard when number 0 is typed.
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
		int n=1;
		while(n>0){
			if(0>read(p_to_A[0],&n,sizeof(int)))
			{
				perror("Error: la citirea numarului in procesul A ");
			}
			n=n+1;
			if(0>write(A_to_p[1],&n,sizeof(int)))
			{
				perror("Error: la scrierea numarului in A");
			}
		}
		exit(EXIT_SUCCESS);
	}

	//B
        int pidB = fork();
        if(pidB==0)
        {
                close(p_to_B[1]);
                close(B_to_p[0]);
                int n=1;
                while(n>0){
                        if(0>read(p_to_B[0],&n,sizeof(int)))
                        {
                                perror("Error: la citirea numarului in procesul B ");
                        }
                        n=n%10;
                        if(0>write(B_to_p[1],&n,sizeof(int)))
                        {
                                perror("Error: la scrierea numarului in B");
                        }
                }
                exit(EXIT_SUCCESS);
        }
	//parinte
	int n=1;
	int afisare=0;
	while(n>0)
	{
		printf("Dati un numar: ");
		scanf("%d",&n);
		if(n==0) exit(EXIT_SUCCESS);
		//verific cui ii trimit
		if( n % 2 == 0)
		{
			//A
			write(p_to_A[1],&n,sizeof(int));
			read(A_to_p[0],&afisare,sizeof(int));
			printf("A: %d\n",afisare);

		}
		else
		{
			 //B
                        write(p_to_B[1],&n,sizeof(int));
                        read(B_to_p[0],&afisare,sizeof(int));
                        printf("B: %d\n",afisare);

		}

	}
	close(p_to_A[1]);
	close(A_to_p[0]);
	close(p_to_B[1]);
	close(B_to_p[0]);
	wait(0);//A
	wait(0);//B
	return 0;

}
