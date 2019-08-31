#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(){

	pid_t taki , taki2;
	for (int i = 0; i < 10; i++)
	{
		taki = fork();
		if (taki == 0)
			{
				break;
			}

	}

	if (taki == 0){ // an einai paidi

		n = getppid();
		waitpid(n)

	}
	if ( taki == 0)
		printf("im a child with pid %d and ppid %d\n",getpid(),getppid());
	else 
	   	printf("im a parent\n");


	//fflush(stdout);
	//while(1){}
	   return 0;
	   //den termatizoyn ola mazi logo xronoprogrammatismou sistimatos
}
