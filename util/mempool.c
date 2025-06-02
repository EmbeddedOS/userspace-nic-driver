#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <stddef.h>
#include <fcntl.h>
#include <stdlib.h>

#include <log.h>
#include <utility.h>
#include <mempool.h>

/* Private function prototypes -----------------------------------------------*/
static char *generate_unique_filepath(char *buf, int len);

static inline int vfio_mmap_2mb_page(uint32_t length, uint8_t **addr);

static int mmap_2mb_page(uint32_t length, struct mem *mem);

/* Private function definitions ----------------------------------------------*/
static char *generate_unique_filepath(char *buf, int len)
{
    char random_filename[10] = {0};

    snprintf(buf, len, "%s/%s", HUGE_PAGE_MOUNT_POINT,
             generate_random_string(random_filename, sizeof(random_filename)));
    return buf;
}

static inline int vfio_mmap_2mb_page(uint32_t length, uint8_t **addr)
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

/**
 * @brief   - To request huge pages using mmap system call,
 */
static int mmap_2mb_page(uint32_t length, struct mem *mem)
{
    int res = 0;
    int fd = 0;
    char temp_huge_file[40] = {0};

    generate_unique_filepath(temp_huge_file, sizeof(temp_huge_file));
    log_info("Generated huge backed filename: %s", temp_huge_file);

    res = open(temp_huge_file, O_CREAT | O_RDWR, 0777);
    expr_check_err(res, exit, "Failed to open hugetlbfs file");

    fd = res;

    ftruncate(fd, length);
    expr_check_err(res, ftruncate_failed, "Failed set page size");

    mem->virt = mmap(NULL, length, PROT_READ | PROT_WRITE,
                     MAP_SHARED | MAP_HUGETLB, fd, 0);
    if (mem->virt == MAP_FAILED)
    {
        mem->virt = NULL;
        res = -errno;
        log_error("Failed to map hugetlbfs file, err: %d", res);
        goto mmap_failed;
    }

    res = lock_mem_in_ram(mem->virt, length);
    expr_check_err(res, lock_mem_in_ram_failed, "Failed to lock page swapping");

    mem->phy = virt_to_phy(mem->virt);

    res = 0;
    goto exit_success;

lock_mem_in_ram_failed:
    munmap(mem->virt, length);
exit_success:
mmap_failed:
ftruncate_failed:
    close(fd);
    unlink(temp_huge_file);
exit:
    return res;
}

int allocate_huge_page(uint32_t length, struct mem *mem)
{
    int res = 0;

    /* TODO: Align the length to page size. */

    res = mmap_2mb_page(length, mem);
    expr_check_err(res, exit, "Failed to allocate huge page");

exit:
    return res;
}

/* Public function definitions -----------------------------------------------*/
int allocate_mempool(uint32_t entry_num, uint32_t entry_size,
                     struct mempool *mempool)
{
    int res = 0;
    struct mem mem = {0};

    mempool->entry_size = entry_size;
    mempool->entry_num = entry_num;

    res = allocate_huge_page(mempool->entry_size * mempool->entry_num, &mem);
    expr_check_err(res, exit, "allocate_huge_page failed");
    mempool->addr = mem.virt;
exit:
    return res;
}


/**
 * @brief   - Translating virtual addrress into physical address by mapping the
 *            /proc/self/pagemap file.
 *          - The device have no idea about virtual memory address, we need to
 *            translate to physical memory address and pass to the device.
 * @link    - https://www.kernel.org/doc/Documentation/vm/pagemap.txt
 */
uintptr_t virt_to_phy(uint8_t *virt)
{
    long page_size = get_system_page_size();

    int fd = open("/proc/self/pagemap", O_RDONLY);
    expr_check_err(fd, exit, "Failed to open /proc/self/pagemap");

    close(fd);
exit:
    return 0;
}

/**
 * @brief   - Lock virtual memory into RAM, preventing that memory from being
 *            paged to the swap area.
 */
int lock_mem_in_ram(uint8_t *virt, size_t size)
{
    return mlock(virt, size);
}
