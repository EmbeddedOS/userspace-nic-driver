#pragma once
#include <stdint.h>

/**
 * @brief   1. To use huge pages, we have to mount a pseudo filesystem of type
 *            hugetlbfs.
 *              $ mount -t hugetlbfs none /mnt/huge
 *            The hugetlbfs is RAM-based fs, and every files that create under
 *            this folder will use huge pages.
 *          2. Now to use huge page by mmap() we have 2 choices:
 *              1. Anonymous mapping.
 *                  - No file needed, directly maps memory from the kernel.
 *                  - Must align to huge page size.
 *                  - Cannot share with other processes.
 *              2. File-backed mapping.
 *                  - Need more file operations open(), close(), etc.
 *                  - Can be shared across processes.
 *                  - Require a file under the mount point, and need ftruncate()
 *                    to resize before mapping.
 *
 * @note    - Setting up huge pages should be done as early as possible, before
 *            the memory be more fragmented, this normally done by put commands
 *            inside a rc init files, and the huge page will be setup in the
 *            boot process.
 * @note    - mmap() return contiguous virtual memory pages but the mapped
 *            physical memory pages are not contiguous. So note that when you
 *            allocate more than one page size.
 */
#ifndef HUGE_PAGE_MOUNT_POINT
#define HUGE_PAGE_MOUNT_POINT "/mnt/huge"
#endif

struct mem
{
    uint8_t *virt;
    uintptr_t phy;
};

struct mempool
{
    uint8_t *addr;
    uint32_t entry_size;
    uint32_t entry_num;

    /* A simple stack to manage entries. Entries will be allocated on heap. */
    uint32_t current_idx;
    uint32_t *entries;
};

int allocate_huge_page(uint32_t length, struct mem *mem);

int allocate_mempool(uint32_t entry_num, uint32_t entry_size,
                     struct mempool *mempool);