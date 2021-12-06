#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <iostream>
#include <fstream>
#include "../headers/baseSem.hpp"

//for testing
#include <chrono>
#include <thread>

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
		if (*readercount == 1)
		{
			sem->wait(0);
		}
		sem->signal(1);

		//for testing
		std::cout << "Reading Data..." << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));

		dataFile.open("./data.txt");
		if (dataFile.is_open())
		{
			while (getline(dataFile, data))
			{
				std::cout << "File Data: " << data << std::endl;
			}
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
