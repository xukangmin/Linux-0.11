#include <stdio.h>
#include "share.h"
int main()
{
    int shmid;
    void *shm;
    shm_data *share;
    int i = 0, val;
    if ((shmid = shmget((key_t)1603101, sizeof(shm_data), 0)) < 0)
    {
        perror("shmget error");
        return -1;
    }
    printf("shm id=%d\n", shmid);
    if ((shm = shmat(shmid, NULL, 0)) == (void *)-1)
    {
        perror("shmat error");
        return -1;
    }
    printf("shm address: %X\n", shm);
    share = (shm_data *)shm;
    while (i < 64)
    {
        if (pop(share, &val) == 0)
        {
            printf("\tconsumer: %d\n", val);
            i++;
        }
    }
    if (shmdt(shm) < 0)
    {
        perror("shmdt error");
        return -1;
    }
    if (shmctl(shmid, 0, NULL) < 0)
    {
        perror("shmctl error");
        return -1;
    }
    printf("consumer finished\n");
    return 0;
}