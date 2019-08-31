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

    typedef struct stream {
      // structure within structure
      struct msg in ;
      struct msg out;
    } stream;
    //---------------------------------------------------------------------shared memory-------------------------------------------------------------------------------

      msg * attach(int shmid) {
      msg * shmem = NULL;

      if ((shmem =(msg *) shmat(shmid, NULL, 0)) == NULL) {
        printf("ERROR on mem attach\n");
        _exit(1);
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
            _exit(1);
        }
        else
            printf("successful request\n");
        
        return semid;
    }

    void remove_semaphore(int semid)
    {
        if((semctl(semid, MUTEX, IPC_RMID, 0)) < 0)
        {
            printf("ERROR on semaphore removal\n");
            // do not exit in case of system call failure
        }
        else
            printf("Removed\n");
        if((semctl(semid, 1, IPC_RMID, 0)) < 0)
        {
            printf("ERROR on semaphore removal\n");
            // do not exit in case of system call failure
        }
        else
            printf("Removed\n");
        if((semctl(semid, 2, IPC_RMID, 0)) < 0)
        {
            printf("ERROR on semaphore removal\n");
            // do not exit in case of system call failure
        }
        else
            printf("Removed\n");
        if((semctl(semid, 3, IPC_RMID, 0)) < 0)
        {
            printf("ERROR on semaphore removal\n");
            // do not exit in case of system call failure
        }
        else
            printf("Removed\n");
        if((semctl(semid, 4, IPC_RMID, 0)) < 0)
        {
            printf("ERROR on semaphore removal\n");
            // do not exit in case of system call failure
        }
        else
            printf("Removed\n");
    }

    void down(int semid, int no_of_sem)
    {
        struct sembuf down={no_of_sem, -1, 0};
    
        if((semop(semid, &down, 1)) < 0)
        {
            printf("ERROR in semop()\n");
            _exit(getpid());
        }else
        printf("im down\n");
    }
    
    void up(int semid, int no_of_sem)
    {
        struct sembuf up={no_of_sem, 1, 0};
    
        if((semop(semid, &up, 1)) < 0)
        {
            printf("ERROR in semop()n");
            _exit(getpid());
        }else
            printf("im up\n");

    }
    void init_semaphore_set(int semID) //initialise the semaphores
    {
        union semun arg;
        
        // initialize semaphore to 1 => unlocked
        arg.val=1;
        if((semctl(semID, MUTEX, SETVAL, arg)) < 0) // SETVAL = Setting the value of semval to arg.val 
        {
            printf("Error on intializing MUTEX\n");
            _exit(1);
        }
        else
            printf("MUTEX successful\n");          //mutex 
        
        arg.val = 1;

        // initialize semaphore to 1 => unlocked
        if((semctl(semID, semWC, SETVAL, arg)) < 0)
        {
            printf("Error on intializing semC \n");
            _exit(1);
        }else
            printf("semC successful\n");           //consumer semWriteC
        

        // initialize semaphore to 1 => unlocked
        if((semctl(semID, semWP, SETVAL, arg)) < 0) //producer semWriteP
        {
            printf("Error on intializing semP \n");
            _exit(1);
        }else
            printf("semP successful\n");

        arg.val = 0;
        // initialize semaphore to 1 => unlocked
        if ((semctl(semID, semRC, SETVAL, arg)) < 0)  //consumer READ
        {
            printf("Error intializing \n");
        }else
             printf("semRC successful\n");  
        

        // initialize semaphore to 1 => unlocked
        if ((semctl(semID, semRP , SETVAL, arg)) < 0) // producer READ
        {
            printf("Error intializing \n");
        }else
             printf("semRP successful\n");  
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
      int num_of_childs, K; // poses diergasies P exw ,K = epanalipseis 
      pid_t P;
      pid_t pid;
      msg prod[2];
      msg con[2];
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
        //shm = attach(shmid); // kanw attach stin memory
        if ((shm = shmat(shmid, NULL, 0)) == NULL) {
            printf("ERROR on mem attach\n");
            _exit(1);
        }
     //---------------------------------------------------creating the semaphores----------------------------------------------------------

        SEMID=request_semaphore_set(5); //posoi simaforoi tha dimiourgithoun 
        init_semaphore_set(SEMID);
        //remove_semaphore(SEMID);

     //------------------------------------------------------------------------------------------------------------------------------------   
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

        while(1){
        pid = getpid();

        //--------------------------------------------------- code to read file ------------------------------------------------------------------
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
        sleep(2);
        rline = rand() % (flines + 1);

        rewind(fp); // gia na paei stin arxi pali

        for (i = 0; i < rline; ++i)
        fgets(buf, sizeof(buf), fp); // to random str apothikevetai sto buf[100]
        //------------------------------------------------------------------------------------------------------------------------------------------       
        
        prod[0].pid=pid;
        strncpy(prod[0].buf,buf,100);

        ////////////////////////////
        down(SEMID,semWP); //MPAINW GIA NA KANW WRITE
        printf("Producer %d Struct with pid %d and message is%s\n", pid, prod[0].pid, prod[0].buf);

        memcpy(&shm[0],&prod[0],sizeof(msg)); // kanw copy apo tin topiki mnimi stin shared mem
        ////////////////////////////                
        up(SEMID,semRC);

        //sleep(5);
        

        printf("Wait for consumer\n");
        down(SEMID,semRP);
        memcpy(&prod[1],&shm[1],sizeof(msg)); // pairnw apo tin shared mem to struct 
        printf("Producer %d got Struct %d, %s\n", pid, prod[1].pid, prod[1].buf);
        up(SEMID,semWC);
        }
      } else { // this is the Consumer

            for (int i = 0; i < K ; i++)
            {
                    //sleep(1);
                    down(SEMID,semRC); // O CONSUMER DIAVAZEI TO MINIMA
                    memcpy(&con[0],&shm[0],sizeof(msg)); // pairnw apo tin shared mem to struct 
                    printf("Consumer get Struct %d, %s\n", con[0].pid, con[0].buf);
                    
                    up(SEMID,semWP);
                    //------------------------------------------------------- md5 the string -----------------------------------------------------------------

                    //use unsigned char
                    unsigned char digest[16];
                    char in[256];
                    strcpy(in,con[0].buf);
                    //char *input = buf;
                    int length = strlen(in) -1 ;
                    int ii=0;

                    // don't miss the underscore after MD5
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
                        //con->buf[ii]=digest[ii];
                        sprintf(in+ii*2,"%02x", digest[ii]);
                    }
                    down(SEMID,semWC);
                    strncpy(con[1].buf,in,100);
                    con[1].pid = con[0].pid;
                    printf("Consumer Write %d, %s\n",con[1].pid,con[1].buf);
                    memcpy(&shm[1],&con[1],sizeof(msg)); // to kanw cpy stin topiki mnimi
                    //sleep(3);
                    up(SEMID,semRP);    
        //----------------------------------------------------------------------------------------------------------------------------------------------------

                
            }
             
        }           
      //detach_shared_mem(shm);
      //remove_shared_mem(shmid);

        return 0;
    }
