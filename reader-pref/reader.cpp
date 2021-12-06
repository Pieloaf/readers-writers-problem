#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <iostream>
#include <fstream>
#include "../headers/baseSem.hpp"

#define SEMKEY 1337
#define SHMKEY 80085

int main()
{
	int shmid, *readercount;
	baseSem *sem = new baseSem(SEMKEY, 2);
	std::ifstream dataFile;
	std::string data;

	shmid = shmget(SHMKEY, 4, 0666 | IPC_CREAT);
	readercount = (int *)shmat(shmid, 0, 0);
	*readercount = 0;

	while (1)
	{
		std::cout << "Press Enter to Read Data";
		std::cin.ignore();
		sem->wait(1);
		*readercount += 1;
		std::cout << "Reader Count: " << *readercount << std::endl;
		if (*readercount == 1)
		{
			sem->wait(0);
		}
		sem->signal(1);

		//for testing
		printf("Reading Data...\0");
		std::cin.ignore();

		dataFile.open("./data.txt");
		if (dataFile.is_open())
		{
			while (getline(dataFile, data))
			{
				std::cout << "\rFile Data: " << data << std::endl;
			}
			std::cout << "End of File Data" << std::endl;
			std::cout << "=============================" << std::endl;
			dataFile.close();
		}

		sem->wait(1);
		*readercount -= 1;
		if (*readercount == 0)
		{
			sem->signal(0);
		}
		sem->signal(1);
	}

	return 0;
}
