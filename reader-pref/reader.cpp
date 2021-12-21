#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <iostream>
#include <fstream>
#include "../headers/baseSem.hpp"

// define semaphore and shared memory keys
#define SEMKEY 1337
#define SHMKEY 80085

int main()
{
	// int to store shared mem id
	// and int pointer to readercount
	int shmid, *readercount;
	char cmd;				  // char to store command to quit
	struct shmid_ds shm_info; // struct to store shared mem info

	// create new baseSem obj with key == semkey and sem set of 2
	baseSem sem(SEMKEY, 2);

	std::ifstream dataFile; // file input stream variable
	std::string data;		// file data string variable

	// initialise shared memory of 4 bytes with with key == shmkey
	// excl ensures that the share memory has not been created before
	shmid = shmget(SHMKEY, 4, 0666 | IPC_CREAT | IPC_EXCL);
	// if the share memory has already been created,
	if (shmid == -1)
	{
		// get the block of shared memory with key == shmkey
		shmid = shmget(SHMKEY, 4, 0666);
		// set the readercount pointer to the shared memory
		readercount = (int *)shmat(shmid, NULL, 0);
	}
	else
	{
		// set readercount pointer to first address in shared memory
		readercount = (int *)shmat(shmid, NULL, 0);
		*readercount = 0; // initialise readercount to 0
	}

	while (1)
	{
		// request user input
		std::cout << "Press `Enter` to read data or q to quit: ";

		// read user input
		cmd = getchar();
		if (cmd == 'q') // if user input is q
			break;		// break out of loop

		sem.wait(1);	   // lock sem to adjust readercount
		*readercount += 1; // increment readercount
		// print readercount to terminal
		std::cout << "Reader Count: " << *readercount << std::endl;
		// if reader is the first reader
		if (*readercount == 1)
		{
			sem.wait(0); // lock sem for file access
		}

		sem.signal(1); // release sem to adjust readercount

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
			std::cout << "================" << std::endl;
			dataFile.close(); // close file input stream
		}

		sem.wait(1);	   // lock sem to adjust readercount
		*readercount -= 1; // decrement readercount
		// if reader is the last reader
		if (*readercount == 0)
		{
			// release sem for file access
			sem.signal(0);
		}
		sem.signal(1); // release sem to adjust readercount
	}

	shmdt((void *)readercount);			// detach shared memory
	shmctl(shmid, SHM_STAT, &shm_info); // get details of shared memory

	if (shm_info.shm_nattch == 0) // if no process is attached to shared memory
	{
		shmctl(shmid, IPC_RMID, NULL); // remove shared memory
	}
	return 0;
}
