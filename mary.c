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
 //---------------------------------------------------------------------structs---------------------------------------------------------------------------

	typedef struct msg {
	  int pid;
	  char buf[100];
	} msg;

	typedef struct stream {
	  // structure within structure
	  struct msg in ;
	  struct msg out;
	} stream;

	////////////////////////////

	///////////////////////////
	//---------------------------------------------------------------------shared memory-------------------------------------------------------------------------------
	/*int request_shared_mem(int memsize) {
	  int shmid;

	  if ((shmid = shmget(IPC_PRIVATE, memsize, 0666 | IPC_CREAT)) < 0) {
	    printf("ERROR in shared mem\n");
	    exit(1);
	  }

	  return shmid;
	}*/

	msg * attach(int shmid) {
	  msg * shmem = NULL;

	  if ((shmem =(msg *) shmat(shmid, NULL, 0)) == NULL) {
	    printf("ERROR on mem attach\n");
	    _exit(1);
	  }

	  return shmem;
	}

	/*void create_shared_mem(int ex) {
	  // create the shared memory
	  SHMID = request_shared_mem(ex * sizeof(struct shared_mem));
	  ptr_shmem_struct = (struct shared_mem * )((void * ) attach(SHMID));

	  // cast the shared memory pointer to void gia na mporw na exw ola ta type of data pou mporei na xreiastw		
	  SHMEM = (void * ) ptr_shmem_struct;
	  printf("im here\n");
	}*/

	void detach_shared_mem(msg * addr) {
	  if (shmdt(addr) < 0) {
	    printf("ERROR on mem detach\n");
	  }
	}

	void remove_shared_mem(int shmid) {
	  struct shmid_ds dummy;

	  if ((shmctl(shmid, IPC_RMID, & dummy)) < 0) {
	    printf("ERROR on mem removal\n");
	  }
	}
	//---------------------------------------------------------------------semaphores--------------------------------------------------------------------

	//-----------------------------------------------------------------------main--------------------------------------------------------------------------	
	int main(int argc, char * argv[]) {

	  FILE * fp;
	  char buf[128];
	  int flines;
	  int rline;
	  int i;
	  int shmid;
	  key_t key = 123456;
	  int num_of_childs, K; // poses diergasies P exw ,K = epanalipseis 
	  pid_t P, C;
	  pid_t pid;
	  msg prod;
	  msg *con;
	  msg *shm;
	  //read file text.txt 
	  // text me md5 hash to steknw ston C

	  if (argc != 3) {
	    printf("Error : You need to input 2 positive numbers for N and K\n");
	    exit(-1);
	  }
	  num_of_childs = atoi(argv[1]);
	  K = atoi(argv[2]);
	  printf("i have %d child(s)\n", num_of_childs);

	  //--------------------------------------------creating shared memory-----------------------------------------------------------------
	  if ((shmid = shmget(key, 2*sizeof(msg), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
	    }
	    shm = attach(shmid); // kanw attach stin memory
     //-----------------------------------------------------------------------------------------------------------------------------------
	  for (int i = 0; i < num_of_childs; i++) {
	    // tha kanw fork gia num of childs
	    P = fork();
	    if (P == 0) {
	      break;
	    }
	    printf("fork successful!\n");
	  }

	  //ta processes P
	  if (P == 0) {
	  	pid = getpid();
	    printf("im a child with pid %d and ppid %d\n", getpid(), getppid());

	    //---------------------------------------------------------------------------- code to read file ------------------------------------------------
	    srand(time(NULL));
	    if (!(fp = fopen("atext.txt", "r"))) {
	      printf("Error open input file\n");
	      return (0);
	    }

	    for (flines = 0; fgets(buf, sizeof(buf), fp); ++flines)
	    ;

	    if (flines == 0) {
	      printf("The file is empty\n");
	      return (0);
	    }

	    rline = rand() % (flines + 1);

	    rewind(fp);

	    for (i = 0; i < rline; ++i)
	      fgets(buf, sizeof(buf), fp); // to random str apothikevetai sto buf
	    printf("%s", buf);
	    
	    prod.pid=pid;
	    strncpy(prod.buf, buf,100);

	    printf("Struct %d, %s\n", prod.pid, prod.buf);

	    memcpy(&shm[0],&prod,sizeof(msg)); // kanw copy apo tin topiki mnimi stin shared mem
	    msg *Pprod;
	    sleep(1);
	    memcpy(&Pprod,&shm[1],sizeof(msg)); // pairnw apo tin shared mem to struct 
	    printf("Struct %d, %s\n", Pprod->pid, Pprod->buf);
	    //shm[0].pid = 6;
	    //strncpy(shm[0].buf, buf,100);
	    //----------------------------------------------------------------------------------------------------------------------------------------------------

	  } else { // this is the Consumer
	    printf("im a parent\n");
	    sleep(1);
	    memcpy(&con,&shm,sizeof(msg)); // pairnw apo tin shared mem to struct 
	    printf("Struct %d, %s\n", con->pid, con->buf);
	    con->pid=999;
	    strncpy(con->buf,"AEK",100);
	 	memcpy(&shm[1],&con,sizeof(msg));

	 	}
	  //detach_shared_mem(shm);
	  //remove_shared_mem(shmid);
	}
