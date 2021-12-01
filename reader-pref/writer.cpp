#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <iostream>
#include <fstream>
#include "./headers/baseSem.hpp"

#define SEMKEY 1337
#define SHMKEY 80085

int main()
{
    baseSem *sem = new baseSem(SEMKEY, 2);

    sem->wait(0);

    // write to file

    sem->wait(1);

    return 0;
}