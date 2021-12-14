#pragma once

#ifndef BASESEM_HPP
#define BASESEM_HPP

#include <sys/sem.h>
#include <stdio.h>

// class definition for base semaphore
class baseSem
{
    // public member function definitions
public:
    // overloaded constructor
    baseSem(int key, int nsems);
    ~baseSem(); // default destructor

    // sem wait function definition
    void wait(int sem_num);
    // sem signal function definition
    void signal(int sem_num);

    // private member variable definitions
private:
    // signal and wait semaphore buffer
    struct sembuf Ssembuf, Wsembuf;
    int semid; // int to store semaphore id
};

#endif