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