#include <errno.h>
#include <stdarg.h>
#include <sys/types.h>

#include <linux/sched.h>
#include <linux/kernel.h>

#include <asm/segment.h>

extern int vsprintf(char *buf, const char *fmt, va_list args);
extern int meminfo_read(char *buffer);

int sprintf(char *buf, const char *fmt, ...)
{
    va_list args;
    int i;
    va_start(args, fmt);
    i = vsprintf(buf, fmt, args);
    va_end(args);
    return i;
}

int psinfo_read(char *buffer)
{
    int len = 0;
    int i;
    struct task_struct *p;
    len += sprintf(buffer, "pid\tstate\tfather\tcounter\tstart_time\n");
    for (i = 0; i < NR_TASKS; i++)
    {
        if (!task[i])
            continue;
        len += sprintf(buffer + len, "%d\t%d\t%d\t%d\t%d\n",
                       task[i]->pid,
                       task[i]->state,
                       task[i]->father,
                       task[i]->counter,
                       task[i]->start_time);
    }
    return len;
}

int hdinfo_read(char *buffer)
{
    struct super_block *sb;
    struct buffer_head *bh;
    char *p;
    int i, j, total, used = 0, free = 0, c;
    char tmp;
    sb = get_super(current->root->i_dev);
    total = sb->s_nzones;
    for (i = 0; i < sb->s_zmap_blocks; i++)
    {
        bh = sb->s_zmap[i];
        p = (char *)bh->b_data;
        for (j = 0; j < 1024; j++)
        {
            if (used + free >= total)
                break;
            tmp = p[j];
            for (c = 0; tmp; c++)
                tmp &= tmp - 1;
            used += c;
            free += 8 - c;
        }
    }
    return sprintf(buffer,
                   "total_blocks:\t%d\nfree_blocks:\t%d\nused_blocks:\t%d\ntotal_inodes:\t%d\n",
                   total,
                   total - used,
                   used,
                   sb->s_ninodes);
}

int inodeinfo_read(char *buffer)
{
    return -EINVAL;
}

int procfs_read(int dev, off_t *pos, char *buf, int count)
{
    char *buffer;
    int len;

    if (count < 0)
        return -EINVAL;
    buffer = (char *)malloc(1024);
    if (!buffer)
        return -ENOMEM;

    switch (dev)
    {
    case 0:
        len = psinfo_read(buffer);
        break;
    case 1:
        len = meminfo_read(buffer);
        break;
    case 2:
        len = hdinfo_read(buffer);
        break;
    case 3:
        len = inodeinfo_read(buffer);
        break;
    }
    if (len < 0)
    {
        free(buffer);
        return len;
    }
    if (*pos >= len)
    {
        free(buffer);
        return 0;
    }
    if (count + *pos > len)
        count = len - *pos;
    memcpy_tofs(buf, buffer + *pos, count);
    free(buffer);
    *pos += count;
    return count;
}
