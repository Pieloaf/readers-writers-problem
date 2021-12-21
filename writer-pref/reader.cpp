#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <iostream>
#include <fstream>
#include "../headers/baseSem.hpp"

// define semaphore and shared memory keys
#define SEMKEY 1338
#define SHMKEY 80086

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

int main()
{
	// int to store shared mem id
	// and int pointer to readercount
	int shmid, *readercount;
	struct shmid_ds shm_info;
	char cmd; // char to store command to quit

	// create new baseSem obj with key == semkey and sem set of 4
	baseSem sem(SEMKEY, 4);

	std::ifstream dataFile; // file input stream variable
	std::string data;		// file data string variable

	// initialise shared memory of 5 bytes with with key == shmkey
	shmid = shmget(SHMKEY, 4, 0666 | IPC_CREAT);
	// set readercount pointer to first address in shared memory
	readercount = (int *)shmat(shmid, 0, 0);
	*readercount = 0; // initialise readercount to 0

	while (1)
	{
		// request user input
		std::cout << "Press Enter to Read Data";

		// read user input
		cmd = getchar();
		if (cmd == 'q') // if user input is q
			break;		// break out of loop

		sem.wait(READTRY); // lock readTry
		sem.wait(RMUTEX);  // lock sem to adjust readercount
		*readercount += 1; // increment the readercount
		// print the readercount to the terminal
		std::cout << "Reader Count: " << *readercount << std::endl;

		// if the reader is the first reader
		if (*readercount == 1)
		{
			sem.wait(FILEACCESS); // lock the sem for file access
		}
		sem.signal(RMUTEX);	 // release sem to adjust readercount
		sem.signal(READTRY); // release readTry

		// For Testing:
		// wait for user input before carrying out CS
		printf("Reading Data...\0");
		std::cin.ignore();

		// open data file
		dataFile.open("./data.txt");
		// when file is ready
		if (dataFile.is_open())
		{
			std::cout << "\rFile Data: " << std::endl;
			// while file is not empty get each line and store it in data variable
			while (getline(dataFile, data))
			{
				// print each line of file
				std::cout << data << std::endl;
			}
			// print message to signify end of reading data
			std::cout << "End of File Data" << std::endl;
			std::cout << "=============================" << std::endl;
			dataFile.close(); // close file input stream
		}

		sem.wait(RMUTEX);  // lock sem to adjust readercount
		*readercount -= 1; // decrement readercount
		// if the reader is the last reader
		if (*readercount == 0)
		{
			// release the sem for file access
			sem.signal(FILEACCESS);
		}
		sem.signal(RMUTEX); // release sem to adjust readercount
	}

	shmdt((void *)readercount);			// detach shared memory
	shmctl(shmid, SHM_STAT, &shm_info); // get details of shared memory

	if (shm_info.shm_nattch == 0) // if no process is attached to shared memory
	{
		shmctl(shmid, IPC_RMID, NULL); // remove shared memory
	}
	return 0;
}
