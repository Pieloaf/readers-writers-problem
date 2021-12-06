#pragma once

#ifndef BASESEM_HPP
#define BASESEM_HPP

#include <sys/sem.h>
#include <stdio.h>

class baseSem
{
private:
    struct sembuf Ssembuf, Wsembuf;
    int semid;

public:
    baseSem(int key, int nsems);
    ~baseSem();

    void wait(int sem_num);
    void signal(int sem_num);
};

#endif