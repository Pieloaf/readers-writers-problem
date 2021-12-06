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
    baseSem *sem = new baseSem(SEMKEY, 2);
    std::ofstream dataFile;
    std::string message;

    while (1)
    {
        std::cout << "Enter Message: ";
        std::cin >> message;
        sem->wait(0);

        //for testing
        printf("Writing Data...\0");
        std::cin.ignore();
        std::cin.ignore();

        dataFile.open("./data.txt");
        if (dataFile.is_open())
        {
            dataFile << message;
            dataFile.close();
        }
        std::cout << "\rData Written: " << message << std::endl;
        std::cout << "=============================" << std::endl;
        sem->signal(0);
    }
    return 0;
}
