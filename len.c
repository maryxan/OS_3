#include <stdio.h>

	#include <stdlib.h>

	#include <unistd.h>

	#include <sys/types.h>

	#include <sys/wait.h>

	#include "header.h"

	#include <sys/ipc.h>

	#include <sys/sem.h>

	#include <sys/shm.h>

	#include <sys/signal.h>

	#include <sys/times.h>

	#include <errno.h>

	#include <time.h>

	#include <string.h>

	#include <openssl/md5.h>

	#include <stdint.h>
	
	#include <openssl/hmac.h>

	#include <semaphore.h>

int main(){

		char *input = "This is tutorialspoint.com";
	    printf("input is %s\n",input);
	    int length = strlen(input);
	    printf("length is %d\n",length);

	    return 0;
}