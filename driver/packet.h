#pragma once
#include <mempool.h>

/**
 * @brief   - The socket mempool structure.
 * @param   - mempool - Huge DMA memory to manage socket buffers.
 * 
 * @param   - current_idx - stack pointer.
 * @param   - free_indexes - List of free buffer indexes.
 * @note    - Simple top down stack implementation to manage free socket buffer
 *            indexes. When we allocate new buffer, we pop index from the stack,
 *            in return, we push the index to free pool when we free the buffer.
 */
struct sk_mempool
{
    struct mempool mempool;

    uint32_t current_index;
    uint32_t *free_indexes;
};

/**
 * @brief   - The socket buffer structure.
 * @param   - phy_addr - Physical memory address to pass to the NIC.
 *
 */
struct sk_buf
{
    struct sk_mempool *sk_mempool;
    uintptr_t phy_addr;

    uint32_t mem_pool_idx;
    uint32_t buf_size;

    uint8_t *buf;
};

int allocate_sk_mempool(struct sk_mempool *sk_mempool, uint32_t sk_buf_num,
                        uint32_t sk_buf_size);

int free_sk_buf(struct sk_buf *sk_buf);

int allocate_sk_buf(struct sk_mempool *sk_mempool, struct sk_buf **sk_buf);

int allocate_sk_buf_batch(struct sk_mempool *sk_mempool,
                          struct sk_buf **sk_buf, uint32_t num);