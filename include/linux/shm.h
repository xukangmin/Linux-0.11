#ifndef _LINUX_SHM_H_
#define _LINUX_SHM_H_

typedef int key_t;

#define IPC_PRIVATE ((key_t) 0)  

/* resource get request flags */
#define IPC_CREAT  00001000   /* create if key is nonexistent */
#define IPC_EXCL   00002000   /* fail if key exists */
#define IPC_NOWAIT 00004000   /* return error on wait */

/* 
 * Control commands used with semctl, msgctl and shmctl 
 * see also specific commands in sem.h, msg.h and shm.h
 */
#define IPC_RMID 0     /* remove resource */
#define IPC_SET  1     /* set ipc_perm options */
#define IPC_STAT 2     /* get ipc_perm options */
#define IPC_INFO 3     /* see ipcs */

struct shmid_ds {
	key_t key;
	unsigned long page;
	unsigned int size;		/* size of segment (bytes) */
	short nattch;		/* no. of current attaches */
};

#define SHMMNI (1<<6)		/* max num of segs system wide */

#endif /* _LINUX_SHM_H_ */