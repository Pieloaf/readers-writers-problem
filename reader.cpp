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
    int shmid, *readcount;
    baseSem *sem = new baseSem(SEMKEY, 2);

    shmid = shmget(SHMKEY, 1, 0666 | IPC_CREAT);
    readcount = (int *)shmat(shmid, 0, 0);
    *readcount = 0;

    sem->wait(1);
    readcount++;

    if (*readcount == 1)
    {
        sem->wait(0);
    }
    sem->signal(1);

    // read from file

    sem->wait(1);
    readcount--;
    if (*readcount == 0)
    {
        sem->signal(0);
    }
    sem->signal(1);

    return 0;
}
