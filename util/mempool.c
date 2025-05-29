#include <sys/mman.h>
#include <stddef.h>

#include <log.h>
#include <mempool.h>

static inline int vfio_mmap_2mb_page(int32_t length, uint8_t **addr)
{
    *addr = (uint8_t *)mmap(
        NULL, length, PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS | MAP_HUGETLB | MAP_HUGE_2MB,
        -1, 0);

    if (*addr == MAP_FAILED)
    {
        *addr = NULL;
        return -errno;
    }

    return 0;
}

static int mmap_2mb_page(int32_t length, uint8_t **addr)
{
    int res = 0;
    int fd = 0;
    res = open();
}


int mempool_create(uint32_t length)
{
    int res = 0;
    uint8_t *virt_huge_mem = NULL;

    res = vfio_mmap_2mb_page(length, &virt_huge_mem);
    expr_check_err(res, exit, "Failed to allocate huge page");

exit:
    return res;
}