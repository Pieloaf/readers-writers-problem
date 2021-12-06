#include "baseSem.hpp"

baseSem::baseSem(int key, int nsems)
{
    semid = semget(key, nsems, 0666 | IPC_CREAT);
    semctl(semid, 0, SETVAL, 1);

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
    printf("piss\n");
    Wsembuf.sem_num = sem_num;
    semop(semid, &Wsembuf, 1);
}

void baseSem::signal(int sem_num)
{
    Ssembuf.sem_num = sem_num;
    semop(semid, &Ssembuf, 1);
}