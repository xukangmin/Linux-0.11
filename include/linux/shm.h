#ifndef _LINUX_SHM_H_
#define _LINUX_SHM_H_

typedef int key_t;

struct shmid_ds {
	key_t key;
	unsigned long page;
	unsigned int size;		/* size of segment (bytes) */
	short nattch;		/* no. of current attaches */
};

#define SHMMNI (1<<6)		/* max num of segs system wide */

#endif /* _LINUX_SHM_H_ */