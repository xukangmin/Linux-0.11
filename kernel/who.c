#include <string.h>
#include <errno.h>
#include <asm/segment.h>

int len = 0;
unsigned char im[24];

int sys_iam(const char *name)
{
    unsigned char buf[24];
    int i = 0;
    for (;;)
    {
        unsigned char tmp = get_fs_byte(&name[i]);
        buf[i] = tmp;
        i++;
        if (tmp == '\0')
        {
            break;
        }
        if (i == 24)
        {
            return -EINVAL;
        }
    }
    len = i - 1;
    for (; i >= 0; i--)
    {
        im[i] = buf[i];
    }
    return len;
}

int sys_whoami(char *name, unsigned int size)
{
    if (size<len)
    {
        return -EINVAL;
    }
    int i = 0;
    for (i=0;i<len;i++)
    {
        put_fs_byte(im[i], &name[i]);
    }
    return len;
}