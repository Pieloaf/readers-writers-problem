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
    baseSem *sem = new baseSem(SEMKEY, 2);
    std::ofstream dataFile;
    std::string message;

    while (1)
    {
        std::cout << "Enter Message: ";
        std::cin >> message;
        sem->wait(0);

        //for testing
        std::cout << "Writing Data..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));

        dataFile.open("./data.txt");
        if (dataFile.is_open())
        {
            dataFile << message;
            dataFile.close();
        }
        sem->signal(0);
    }
    return 0;
}
