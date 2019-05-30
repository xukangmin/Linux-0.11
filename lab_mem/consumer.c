#include <stdio.h>
#include "share.h"
int main()
{
    int shmid;
    void *shm;
    shm_data *share;
    int i;
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
    for (i = 0; i < 64; i++)
    {
        int val;
        if (pop(share, &val) < 0)
        {
            i--;
        }else{
            printf("\tconsumer: %d\n",val);
        }
    }
    if (shmdt(shm)<0){
        perror("shmdt error");
        return -1;
    }
    printf("consumer finished\n");
    return 0;
}