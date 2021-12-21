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
    // create new baseSem obj with key == semkey and sem set of 2
    baseSem sem(SEMKEY, 2);
    std::ofstream dataFile; // file output stream variable
    std::string message;    // file data string variable

    while (1)
    {
        // request user to enter data for file
        std::cout << "Enter Message or 'q' to quit: ";
        // read input from stdin to message variable
        std::cin >> message;

        // if user enters 'q' then quit
        if (message == "q")
            break;

        sem.wait(0); // lock sem for file access

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
            dataFile.close();    // close file output stream
        }
        // print message to signify end of writing data
        std::cout << "\rData Written: " << message << std::endl;
        std::cout << "=============================" << std::endl;
        sem.signal(0); // release sem for file access
    }
    return 0;
}
