#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <iostream>
#include <fstream>
#include "../headers/baseSem.hpp"

#define SEMKEY 1338
#define SHMKEY 80086

int main()
{
	int shmid, *readercount;
	baseSem *sem = new baseSem(SEMKEY, 4);
	std::ifstream dataFile;
	std::string data;

	shmid = shmget(SHMKEY, 4, 0666 | IPC_CREAT);
	readercount = (int *)shmat(shmid, 0, 0);
	*readercount = 0;

	while (1)
	{
		std::cout << "Press Enter to Read Data";
		std::cin.ignore();
		sem->wait(2); // lock readTry
		sem->wait(0); // lock rmutex
		*readercount += 1;
		if (*readercount == 1)
		{
			sem->wait(3); // lock resource
			std::cout << "Resource Locked" << std::endl;
		}
		sem->signal(0); // release rmutex
		sem->signal(2); // release readTry

		//for testing
		std::cout << "Reading Data..." << std::endl;
		std::cin.ignore();

		dataFile.open("./data.txt");
		if (dataFile.is_open())
		{
			while (getline(dataFile, data))
			{
				std::cout << "File Data: " << data << std::endl;
			}
			dataFile.close();
		}

		sem->wait(0); // lock rmutex
		*readercount -= 1;
		if (*readercount == 0)
		{
			sem->signal(3); // release resource
			std::cout << "Resource Released" << std::endl;
		}
		sem->signal(0); // release rmutex
	}

	return 0;
}
