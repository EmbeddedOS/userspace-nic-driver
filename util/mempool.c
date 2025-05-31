#include <sys/mman.h>
#include <stddef.h>
#include <fcntl.h>
#include <stdlib.h>

#include <log.h>
#include <utility.h>
#include <mempool.h>

static char *generate_unique_filepath(char *buf, int len)
{
    char random_filename[10] = {0};

    snprintf(buf, len, "%s/%s", HUGE_PAGE_MOUNT_POINT,
             generate_random_string(random_filename, sizeof(random_filename)));
    return buf;
}

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
    char temp_huge_file[40] = {0};

    generate_unique_filepath(temp_huge_file, sizeof(temp_huge_file));
    log_info("Generated huge backed file: %s", temp_huge_file);


    res = open(temp_huge_file, O_CREAT | O_EXCL | O_RDWR, 0777);
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