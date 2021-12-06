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
    int shmid, *readcount;
    baseSem *sem = new baseSem(SEMKEY, 2);
    std::ifstream dataFile;
    printf("oof\n");
    shmid = shmget(SHMKEY, 4, 0666 | IPC_CREAT);
    readcount = (int *)shmat(shmid, 0, 0);
    *readcount = 0;
    printf("rcpepe\n");
    printf("%x\n",readcount);
    sem->wait(1);
    readcount++;
    printf("%d\n",readcount);
    if (*readcount == 1)
    {
        printf("here?\n");
        sem->wait(0);
    }
    sem->signal(1);

    printf("haha\n");

    sem->wait(1);
    readcount--;
    if (*readcount == 0)
    {
        sem->signal(0);
    }
    sem->signal(1);
    printf("pee\n");

    return 0;
}
