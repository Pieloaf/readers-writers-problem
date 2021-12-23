#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <iostream>
#include <fstream>
#include "../headers/baseSem.hpp"

// define semaphore and shared memory keys
#define SEMKEY 1338
#define SHMKEY 80087

/*
	sem nums:
		0 - adjust readercount
		1 - adjust writercount
		2 - readTry
		3 - file access
*/

#define RMUTEX 0
#define WMUTEX 1
#define READTRY 2
#define FILEACCESS 3
#define MEMMGMT 4

int main()
{
	// int to store shared mem id
	// and int pointer to writercount
	int shmid, *writercount;
	struct shmid_ds shm_info;
	// create new baseSem obj with key == semkey and sem set of 4
	baseSem sem(SEMKEY, 5);

	std::ofstream dataFile; // file output stream variable
	std::string message;	// file data string variable

	sem.wait(MEMMGMT); // lock semaphore for shared memory management
	// initialise shared memory of 4 bytes with with key == shmkey
	shmid = shmget(SHMKEY, 4, 0666 | IPC_CREAT | IPC_EXCL);
	// if the share memory has already been created,
	if (shmid == -1)
	{
		// get the block of shared memory with key == shmkey
		shmid = shmget(SHMKEY, 4, 0666);
		// set the writercount pointer to the shared memory
		writercount = (int *)shmat(shmid, NULL, 0);
	}
	else
	{
		// set writercount pointer to first address in shared memory
		writercount = (int *)shmat(shmid, NULL, 0);
		*writercount = 0; // initialise writercount to 0
	}
	sem.signal(MEMMGMT); // signal semaphore for shared memory management

	while (1)
	{
		// request user to enter data for file
		std::cout << "Enter Message: ";
		// read input from stdin to message variable
		std::cin >> message;

		// if user enters 'q' then quit
		if (message == "q")
			break;

		sem.wait(WMUTEX);  // lock sem to adjust writercount
		*writercount += 1; // increment writercount
		// print writercount to terminal
		std::cout << "Writer Count: " << *writercount << std::endl;

		// if writer is the first writer
		if (*writercount == 1)
		{
			// lock sem to prevent new readers from inc readercount
			sem.wait(READTRY);
		}

		sem.signal(WMUTEX);	  // release sem to adjust writercount
		sem.wait(FILEACCESS); // lock sem for file access

		// For Testing:
		// wait for user input before carrying out CS
		printf("Writing Data...\0");
		std::cin.ignore();
		std::cin.ignore();

		// open data file
		dataFile.open("./data.txt");
		// when file is ready
		if (dataFile.is_open())
		{
			dataFile << message; // write data to file
			dataFile.close();	 // close file output stream
		}
		// print message to signify end of writing data
		std::cout << "\rData Written: " << message << std::endl;
		std::cout << "=============================" << std::endl;

		sem.signal(FILEACCESS); // release sem for file access
		sem.wait(WMUTEX);		// lock sem to adjust writercount
		*writercount -= 1;		// decrement writercount
		// if writer is the last writer
		if (*writercount == 0)
		{
			// release sem to allow new readers inc readercount
			sem.signal(READTRY);
		}
		sem.signal(WMUTEX); // release sem to adjust writercount
	}

	sem.wait(MEMMGMT); // lock semaphore for shared memory management

	shmdt((void *)writercount);			// detach shared memory
	shmctl(shmid, SHM_STAT, &shm_info); // get details of shared memory

	if (shm_info.shm_nattch == 0) // if no process is attached to shared memory
	{
		shmctl(shmid, IPC_RMID, NULL); // remove shared memory
	}
	sem.signal(MEMMGMT); // signal semaphore for shared memory management
	return 0;
}
