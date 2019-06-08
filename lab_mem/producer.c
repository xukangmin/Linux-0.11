#include <stdio.h>
#include <string.h>
#include "share.h"

int main()
{
    int shmid;
    void *shm;
    shm_data *share;
    int i = 0;
    if ((shmid = shmget((key_t)1603101, sizeof(shm_data), IPC_CREAT)) < 0)
    {
        perror("shmget error");
        return -1;
    }
    printf("shm id=%d\n", shmid);
    if ((shm = (void *)shmat(shmid, NULL, 0)) == (void *)-1)
    {
        perror("shmat error");
        return -1;
    }
    printf("shm address: %X\n", shm);
    memset(shm, 0x0, sizeof(shm_data));
    share = (shm_data *)shm;
    while (i < 64)
    {
        if (push(share, i) == 0)
        {
            printf("PRODUCER: %d\n", i);
            i++;
        }
    }
    if (shmdt(shm) < 0)
    {
        perror("shmdt error");
        return -1;
    }
    printf("PRODICER FINISHED\n");
    return 0;
}