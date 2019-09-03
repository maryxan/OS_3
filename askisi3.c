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

    //---------------------------------------------------------------------structs---------------------------------------------------------------------------
    typedef struct msg {
      int pid;
      char buf[100];
    } msg;
    //---------------------------------------------------------------------shared memory-------------------------------------------------------------------------------

      msg * attach(int shmid) {
      msg * shmem = NULL;

      if ((shmem =(msg *) shmat(shmid, NULL, 0)) == NULL) {
        printf("ERROR on mem attach\n");
        exit(1);
      }

      return shmem;
    }

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
    int request_semaphore_set(int num_of_semaphores)
    {
        int semid;
        if((semid=semget(IPC_PRIVATE, num_of_semaphores, 0666 | IPC_CREAT)) < 0)
        {
            printf("ERROR on sem set\n");
            exit(1);
        }
        
        return semid;
    }

    void remove_semaphore(int semid)
    {
        if((semctl(semid, MUTEX1, IPC_RMID, 0)) < 0)
        {
            printf("ERROR on semaphore removal\n");
        }
             
    }

    void down(int semid, int no_of_sem)
    {
        struct sembuf down={no_of_sem, -1, 0};
    
        if((semop(semid, &down, 1)) < 0)
        {
            printf("ERROR in semop()\n");
            exit(getpid());
        }
    }
    
    void up(int semid, int no_of_sem)
    {
        struct sembuf up={no_of_sem, 1, 0};
    
        if((semop(semid, &up, 1)) < 0)
        {
            printf("ERROR in semop()n");
            exit(getpid());
        }
    }
    void init_semaphore_set(int semID) //initialise the semaphores
    {
        union semun arg;
        
        arg.val=1;
        if((semctl(semID, MUTEX1, SETVAL, arg)) < 0) // SETVAL = Setting the value of semval to arg.val 
        {
            printf("Error on intializing MUTEX\n");
            exit(1);
        }
        
        if((semctl(semID, MUTEX2, SETVAL, arg)) < 0) 
        {
            printf("Error on intializing MUTEX\n");
            exit(1);
        }
        
        if((semctl(semID, semWC, SETVAL, arg)) < 0) //consumer WRITE
        {
            printf("Error on intializing semC \n");
            exit(1);
        }                                            
        
        if((semctl(semID, semWP, SETVAL, arg)) < 0) //producer WRITE
        {
            printf("Error on intializing semP \n");
            exit(1);
        }

        arg.val = 0;
        if ((semctl(semID, semRC, SETVAL, arg)) < 0)  //consumer READ
        {
            printf("Error intializing \n");
            exit(1);
        } 
        
        if ((semctl(semID, semRP , SETVAL, arg)) < 0) // producer READ
        {
            printf("Error intializing \n");
            exit(1);

        } 
    }
    int PID_MATCH = 0;
    void sigintHandler(int sig_num) { 
            signal(SIGQUIT, sigintHandler); 
        }
    void sigintHandler2(int sig_num) { 
            signal(SIGQUIT, sigintHandler); 
            printf("exit child %d\n", PID_MATCH);
            exit(PID_MATCH);
        }

    //-----------------------------------------------------------------------main-------------------------------------------------------------------------- 
    int main(int argc, char * argv[]) {

      FILE * fp;
      char buf[128];
      int flines;
      int rline;
      int i;
      int shmid;
      key_t key = 123456;
      int num_of_childs, K;
      pid_t P;
      pid_t pid;
      msg prod[2];
      msg con[2];
      msg *shm;
      unsigned char digest[16];
      char in[256];

      if (argc != 3) {
        printf("Error : You need to input 2 positive numbers for N and K\n");
        exit(-1);
      }
      num_of_childs = atoi(argv[1]);
      K = atoi(argv[2]);
      printf("STARTING THE PROGRAM WITH %d PROCESSES \n", num_of_childs);
      srand(time(NULL));
      //--------------------------------------------creating and attaching shared memory-----------------------------------------------------------------

      if ((shmid = shmget(key, 2*sizeof(msg), IPC_CREAT | 0666)) < 0) {
        perror("ERROR on creating shared memory");
        exit(1);
        }
        if ((shm = shmat(shmid, NULL, 0)) == NULL) {
            printf("ERROR on memory attach\n");
            exit(1);
        }
     //---------------------------------------------------creating the semaphores----------------------------------------------------------

        SEMID=request_semaphore_set(6); //creating 6 semaphores 
        init_semaphore_set(SEMID);
        
     //------------------------------------------------------------------------------------------------------------------------------------   
      for (int i = 0; i < num_of_childs; i++) {
        P = fork();
        if (P == 0) {
          break;
        }     
      }
      //if (P == 0) is a child process
      if (P == 0) {
        signal(SIGQUIT, sigintHandler2);
        while(1){
        pid = getpid();

    //--------------------------------------------------- getting a random file from text ----------------------------------------------
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
        fgets(buf, sizeof(buf), fp); 
    //------------------------------------------------------------------------------------------------------------------------------------
        
        prod[0].pid=pid;
        strncpy(prod[0].buf,buf,100);

    
        down(SEMID,MUTEX1);
        down(SEMID,semWP); // down to write
        printf("\nProducer with pid %d is writing to struct in with pid %d. The message is \"%s.\"\n", pid, prod[0].pid, prod[0].buf);

        memcpy(&shm[0],&prod[0],sizeof(msg)); // cpy from local mem to shared mem                
        up(SEMID,semRC);
        up(SEMID,MUTEX1);

        

        printf("Waiting for Consumer..\n");
        down(SEMID,MUTEX2);
        down(SEMID,semRP);
        memcpy(&prod[1],&shm[1],sizeof(msg)); // getting the struct from shared mem
        printf("Producer with pid %d got struct out with pid %d, and message \"%s.\"\n", pid, prod[1].pid, prod[1].buf);
        if (prod[1].pid == pid)
        {
            PID_MATCH++;
            printf("Pids are matching!\n");
        }
        up(SEMID,semWC);
        up(SEMID,MUTEX2);
        }
      } else { // this is the Consumer

            for (int i = 0; i < K ; i++)
            {
                down(SEMID,semRC); // down so consumer can read the struct
                memcpy(&con[0],&shm[0],sizeof(msg)); // getting the struct from shared mem 
                printf("Consumer got the struct in with pid %d. The message he got is \"%s.\" \n", con[0].pid, con[0].buf);
                
                up(SEMID,semWP);
                //------------------------------------------------------- md5 the string -----------------------------------------------------------------
                strcpy(in,con[0].buf);
                int length = strlen(in) -1 ;
                int ii=0;

                MD5_CTX md5;    
                MD5_Init(&md5);

                while (length > 0) {
                    if (length > 512) {
                        MD5_Update(&md5, in, 512);
                    } else {
                        MD5_Update(&md5, in, length);
                    }
                    length -= 512;
                }

                MD5_Final(digest, &md5);
                for(ii = 0; ii < 16; ii++) {
                    sprintf(in+ii*2,"%02x", digest[ii]);
                }
                down(SEMID,semWC);
                strncpy(con[1].buf,in,100);
                con[1].pid = con[0].pid;
                printf("Consumer Wrote in struct out with pid %d, the encrypted message \"%s.\"\n",con[1].pid,con[1].buf);
                memcpy(&shm[1],&con[1],sizeof(msg)); // cpy to shared mem
                up(SEMID,semRP);    
                //----------------------------------------------------------------------------------------------------------------------------------------------------

            
            }
                signal(SIGQUIT, sigintHandler);
                kill(0,SIGQUIT);
                int exit_pid_match=0;
                int sum = 0;
                for(int i=0; i<num_of_childs; i++){
                    waitpid(-1, &exit_pid_match,0);
                    sum+=WEXITSTATUS(exit_pid_match);
                }
                sleep(2);
                printf("THE STATISTICS ARE:\n");
                printf("==========================================================\n");

                printf("The number of P processes are: %d \n",num_of_childs);
                printf("The sum of the pid_match is: %d\n", sum);

                printf("==========================================================\n");
            } 

            
            remove_semaphore(SEMID);           
            detach_shared_mem(shm);
            remove_shared_mem(shmid);

    return 0;
    }

