#include "baseSem.hpp"

baseSem::baseSem(int key, int nsems)
{
    semid = semget(key, nsems, 0666 | IPC_CREAT);
    for (int i = 0; i < nsems; i++)
    {
        semctl(semid, i, SETVAL, 1);
    }
    Wsembuf.sem_op = -1;
    Wsembuf.sem_flg = SEM_UNDO;

    Ssembuf.sem_op = 1;
    Ssembuf.sem_flg = SEM_UNDO;
}

baseSem::~baseSem()
{
}

void baseSem::wait(int sem_num)
{
    Wsembuf.sem_num = sem_num;
    semop(semid, &Wsembuf, 1);
}

void baseSem::signal(int sem_num)
{
    Ssembuf.sem_num = sem_num;
    semop(semid, &Ssembuf, 1);
}