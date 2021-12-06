#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <iostream>
#include <fstream>
#include "../headers/baseSem.hpp"

#define SEMKEY 1338
#define SHMKEY 80087

int main()
{
	int shmid, *writercount;
	baseSem *sem = new baseSem(SEMKEY, 4);
	std::ofstream dataFile;
	std::string message;

	shmid = shmget(SHMKEY, 4, 0666 | IPC_CREAT);
	writercount = (int *)shmat(shmid, 0, 0);
	*writercount = 0;

	while (1)
	{
		std::cout << "Enter Message: ";
		std::cin >> message;
		sem->wait(1); // lock wmutex
		*writercount += 1;
		std::cout << *writercount << std::endl;
		if (*writercount == 1)
		{
			sem->wait(2); // lock readTry
		}
		sem->signal(1); // release wmutex
		sem->wait(3);	// lock resource
		std::cout << "Resource Locked" << std::endl;

		//for testing
		std::cout << "Writing Data..." << std::endl;
		std::cin.ignore();
		std::cin.ignore();

		dataFile.open("./data.txt");
		if (dataFile.is_open())
		{
			dataFile << message;
			dataFile.close();
		}

		sem->signal(3); // release resource
		sem->wait(1);	// lock wmutex
		*writercount -= 1;
		if (*writercount == 0)
		{
			sem->signal(2); //release readTry
		}
		sem->signal(1); // lock wmutex
	}
	return 0;
}
