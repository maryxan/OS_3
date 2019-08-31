union semun
{
	int val; /* value for SETVAL */
	struct semid_ds *buff; /* buffer for IPC_STAT, IPC_SET */
	unsigned short *array; /* array for GETALL, SETALL */
};
// struct to be used in a shared segment
// (actually, an array of such structs... one for each product)
struct shared_mem
{
	pid_t creator_pid; // the pid of each creator
	pid_t inspector_pid; // the pid of each inspector
}shmem_struct, *ptr_shmem_struct;

/*
 * Start MD5 accumulation.  Set bit count to 0 and buffer to mysterious
 * initialization constants.
 */

int MUTEX=0 , semWC = 1 ,semWP = 2 , semRC = 3 , semRP = 4;
int SEMID=0;
int SHMID=0;
void *SHMEM=NULL;
int SLEEPTIME=1;
int ex;
