union semun
{
	int val; /* value for SETVAL */
	struct semid_ds *buff; /* buffer for IPC_STAT, IPC_SET */
};

int MUTEX1=0 , semWC = 1 ,semWP = 2 , semRC = 3 , semRP = 4 ,MUTEX2 = 5;
int SEMID=0;
int SHMID=0;
//void *SHMEM=NULL;

