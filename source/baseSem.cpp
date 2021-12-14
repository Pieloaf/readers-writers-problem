#include "baseSem.hpp"

// Overloaded constructor implementation
baseSem::baseSem(int key, int nsems)
{
	// get semaphore set identifier of `nsems` number of semaphores
	// with with read-write permissions for everyone
    semid = semget(key, nsems, 0666 | IPC_CREAT);
	
	// for each semaphore
    for (int i = 0; i < nsems; i++)
    {
		// initialise to 1 (unblocked)
        semctl(semid, i, SETVAL, 1);
    }
	
	// Initialise sem op buffers //
	// Wait Semaphore
    Wsembuf.sem_op = -1; // decrement sem value when called
    Wsembuf.sem_flg = SEM_UNDO; // set SEM_UNDO flag

	// Signal Semaphore 
    Ssembuf.sem_op = 1; // increment sem value when called
    Ssembuf.sem_flg = SEM_UNDO; // set SEM_UNDO flag
}

// default destructor
baseSem::~baseSem()
{
}

// implementation of wait function
void baseSem::wait(int sem_num)
{
	// set sem number to perform operation on
    Wsembuf.sem_num = sem_num;
	// call sem wait operation on sem set of id == semid
    semop(semid, &Wsembuf, 1);
}

// implementation of signal function
void baseSem::signal(int sem_num)
{
	// set sem number to perform operation on
    Ssembuf.sem_num = sem_num;
	// call sem signal operation on sem set of id == semid
    semop(semid, &Ssembuf, 1);
}