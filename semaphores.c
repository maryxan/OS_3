int request_semaphore_set(int numOfSems)
	{
		int semid;
		
		if((semid=semget(IPC_PRIVATE, numOfSems, 0666 | IPC_CREAT)) < 0)
		{
			printf("ERROR on sem set\n");
			_exit(1);
		}
		
		return semid;
	}
	
	void init_semaphore_set(int semID, int products)
	{
		union semun arg;
		int i=0;
		
		// initialize semaphore MuTeX to 1 => unlocked
		arg.val=1;
		if((semctl(semID, MuTeX, SETVAL, arg)) < 0)
		{
			printf("Error on intializing the semaphore\n");
			_exit(1);
		}
		
		// initialize semaphore PaInTiNg to 1 => unlocked
		if((semctl(semID, PaInTiNg, SETVAL, arg)) < 0)
		{
			printf("Error on intializing the semaphore\n");
			_exit(1);
		}
		
		// initialize semaphore screen to 1 => unlocked
		if((semctl(semID, screen, SETVAL, arg)) < 0)
		{
			printf("Error on intializing the semaphore\n");
			_exit(1);
		}
		
		// initialize semaphores for each product to 1 => unlocked
		for(i=0;i<products;i++)
		{
			if((semctl(semID, i+3, SETVAL, arg)) < 0)
			{
				printf("Error on intializing the semaphore\n");
				_exit(1);
			}
		}
	}
	
	void remove_semaphore(int semid)
	{
		if((semctl(semid, 0, IPC_RMID, 0)) < 0)
		{
			printf("ERROR on semaphore removal\n");
			// do not exit in case of system call failure
		}
	}
	
	void down(int semid, int numOfSem)
	{
		struct sembuf down={numOfSem, -1, 0};
	
		if((semop(semid, &down, 1)) < 0)
		{
			printf("ERROR in semop()\n");
			_exit(getpid());
		}
	}
	
	void up(int semid, int numOfSem)
	{
		struct sembuf up={numOfSem, 1, 0};
	
		if((semop(semid, &up, 1)) < 0)
		{
			printf("ERROR in semop()n");
			_exit(getpid());
		}
	}