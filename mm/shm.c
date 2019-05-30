#include <string.h>
#include <sys/types.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/shm.h>
#include <errno.h>

static int shm_tot = 0;   /* total number of shared memory pages */
static int max_shmid = 0; /* every used id is <= max_shmid */
static struct shmid_ds shm_segs[SHMMNI];

/**
 * shmget()会新建/打开一页内存，并返回该页共享内存的shmid（该块共享内存在操作系统内部的id）。
 * 所有使用同一块共享内存的进程都要使用相同的key参数。
 * 如果key所对应的共享内存已经建立，则直接返回shmid。
 * 如果size超过一页内存的大小，返回-1，并置errno为EINVAL。
 * 如果系统无空闲内存，返回-1，并置errno为ENOMEM。
 */
int sys_shmget(key_t key, size_t size, int shmflg)
{
	struct shmid_ds *shp;
	unsigned long p;
	int id = 0;

	if (size < 0 || size > PAGE_SIZE)
		return -EINVAL;
	// key = IPC_PRIVATE not support
	if (key == 0)
		return -ENOSYS;
	for (id = 0; id <= max_shmid; id++)
	{
		if (key == shm_segs[id].key)
		{
			if (size > shm_segs[id].size)
				return -EINVAL;
			printk("[sys_shmget] exist id: %d\n", id);
			return id;
		}
	}

	if (shm_tot == SHMMNI)
		return -ENOSPC;
	for (id = 0; id < SHMMNI; id++)
	{
		if (shm_segs[id].key == 0)
		{
			printk("[sys_shmget] create id: %d\n", id);
			goto found;
		}
	}
	return -ENOSPC;

found:
	p = get_free_page();
	if (!p)
		return -ENOMEM;
	printk("[sys_shmget] alloc page: %X\n", p);
	shm_tot++;
	shm_segs[id].key = key;
	shm_segs[id].page = p;
	shm_segs[id].size = size;
	shm_segs[id].nattch = 0;
	if (id > max_shmid)
		max_shmid = id;
	return id;
}

/**
 * shmat()会将shmid指定的共享页面映射到当前进程的虚拟地址空间中，并将其首地址返回。
 * 如果shmid非法，返回-1，并置errno为EINVAL。shmaddr和shmflg参数可忽略。
 */
void *sys_shmat(int shmid, const void *shmaddr, int shmflg)
{
	unsigned long ds, brk;
	if (shmid < 0 || shmid > max_shmid || shm_segs[shmid].key == 0)
		return -EINVAL;
	ds = get_base(current->ldt[2]);
	brk = current->brk;
	if (put_page(shm_segs[shmid].page, ds + brk) == 0)
		return -ENOMEM;
	printk("[sys_shmat] ds:addr : %X: %X, paddr: %X", ds, brk, shm_segs[shmid].page);
	current->brk = brk + PAGE_SIZE;
	shm_segs[shmid].nattch++;
	return (void *)brk;
}

int sys_shmdt(const void *shmaddr)
{
	unsigned long vaddr,ds, *page_table;
	ds = get_base(current->ldt[2]);
	vaddr = (unsigned long )shmaddr + ds;
	page_table = (unsigned long *) ((vaddr>>20) & 0xffc);
	if ((*page_table)&1)
		page_table = (unsigned long *) (0xfffff000 & *page_table);
	else {
		return EINVAL;
	}
	page_table[(vaddr>>12) & 0x3ff] = NULL;
	return 0;
}