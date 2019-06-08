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
	if (key == IPC_PRIVATE)
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
	printk("[sys_shmat] ds:addr : %X: %X, paddr: %X\n", ds, brk, shm_segs[shmid].page);
	current->brk = brk + PAGE_SIZE;
	shm_segs[shmid].nattch++;
	current->shmid = shmid;
	return (void *)brk;
}

/**
 * shmdt()会将shmaddr指定的页面从当前进程的虚拟地址空间中删除。
 * 如果shmaddr对应的页表项不存在，则返回EINVAL。
 * 传入不是由shmat附加的虚拟地址会产生异常行为。
 */
int sys_shmdt(const void *shmaddr)
{
	unsigned long vaddr, ds, *page_table;
	ds = get_base(current->ldt[2]);
	vaddr = (unsigned long)shmaddr + ds;
	page_table = (unsigned long *)((vaddr >> 20) & 0xffc);
	if ((*page_table) & 1)
		page_table = (unsigned long *)(0xfffff000 & *page_table);
	else
		return EINVAL;
	page_table[(vaddr >> 12) & 0x3ff] = NULL;
	shm_segs[current->shmid].nattch--;
	current->shmid = (key_t)-1;
	return 0;
}

/**
 * shmctl()会将shmid指定的共享页面释放。
 * 如果shmid没有对应的页，则返回EINVAL。
 * 使用前应当先使用shmdt分离虚拟地址。
 * command仅支持IPC_RMID（0）
 * buf被忽略
 */
int sys_shmctl(int shmid, int command, void *buf)
{
	if (shmid < 0 || shmid > max_shmid || shm_segs[shmid].key == 0 || shm_segs[shmid].nattch != 0)
		return -EINVAL;
	if (command != IPC_RMID)
		return -ENOSYS;
	free_page(shm_segs[shmid].page);
	shm_segs[shmid].key = 0;
	shm_segs[shmid].size = 0;
	shm_segs[shmid].page = 0;
	shm_tot--;
	while (max_shmid > 0 && shm_segs[max_shmid].key == 0)
		max_shmid--;
	return 0;
}