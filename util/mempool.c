#include <sys/mman.h>
#include <stddef.h>

#include <mempool.h>

int mempool_create(uint32_t length)
{
    int res = 0;
    res = memfd_create(NULL, NULL);

exit:
    return res;
}