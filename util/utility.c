#include <sys/stat.h>
#include <errno.h>
#include <stdint.h>

#include <utility.h>

int64_t get_filesize(int fd)
{
    struct stat file = {0};
    if (fstat(fd, &file) == -1)
    {
        return -errno;
    }

    return file.st_size;
}

uint8_t *virt_to_phy(uint8_t *virt)
{
    return NULL;
}

char *generate_random_string(char *buf, int len)
{
    static char charset[] = "0123456789"
                            "abcdefghijklmnopqrstuvwxyz"
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *tmp = buf;
    while (len-- > 0)
    {
        size_t index = (double)rand() / RAND_MAX * (sizeof charset - 1);
        *tmp++ = charset[index];
    }
    *tmp = '\0';
    return buf;
}