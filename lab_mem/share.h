#define __LIBRARY__
#include <unistd.h>
#define BITS 4
#define SIZE (1 << BITS)
#define MASK (SIZE - 1)

typedef int key_t;

#ifdef __NR_shmget
_syscall3(int,shmget,key_t,key,unsigned int,size,int,shmflg)
_syscall3(void *,shmat,int,shmid,const void *,shmaddr,int,shmflg)
_syscall1(int,shmdt,const void *,shmaddr)
_syscall3(int,shmctl,int,shmid,int,command,void* ,buf)
#endif
#ifndef IPC_CREAT
#define IPC_CREAT  00001000
#endif

typedef struct share_memory_data
{
    unsigned int in;
    unsigned int out;
    int buffer[SIZE];
} shm_data;

int push(shm_data *mem, int val)
{
    if (mem->in - mem->out >= SIZE)
        return -1;
    mem->buffer[mem->in&MASK] = val;
    mem->in++;
    return 0;
}

int pop(shm_data *mem, int *val)
{
    if (mem->in == mem->out)
        return -1;
    *val = mem->buffer[mem->out&MASK];
    mem->out++;
    return 0;
}