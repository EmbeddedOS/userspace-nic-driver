#include <stdlib.h>

#include <log.h>
#include <packet.h>

/* Private function protototypes ---------------------------------------------*/
static inline struct sk_buf *
get_sk_buf_from_dma_mem(struct mempool *mempool, uint32_t idx);

uint32_t pop_free_indexes_stack(struct sk_mempool *sk_mempool);

void push_free_indexes_stack(struct sk_mempool *sk_mempool, uint32_t index);

/* Private function definitions ----------------------------------------------*/
static inline struct sk_buf *
get_sk_buf_from_dma_mem(struct mempool *mempool, uint32_t idx)
{
    return (struct sk_buf *)(mempool->addr + idx * mempool->entry_size);
}

uint32_t pop_free_indexes_stack(struct sk_mempool *sk_mempool)
{
    sk_mempool->current_index -= 1;
    return sk_mempool->free_indexes[sk_mempool->current_index];
}

void push_free_indexes_stack(struct sk_mempool *sk_mempool, uint32_t index)
{
    sk_mempool->free_indexes[sk_mempool->current_index] = index;
    sk_mempool->current_index += 1;
}

/* Public function definitions -----------------------------------------------*/
int allocate_sk_mempool(struct sk_mempool *sk_mempool, uint32_t sk_buf_num,
                        uint32_t sk_buf_size)
{
    int res = 0;

    /* 1. Allocate DMA. */
    res = allocate_mempool(sk_buf_num, sk_buf_size, &sk_mempool->mempool);

    /* 2. Initialize stack to manage free socket buffer index. */
    sk_mempool->current_index = sk_buf_num;
    sk_mempool->free_indexes = malloc(sk_buf_num * sizeof(uint32_t));

    /* 3. Initialize socket buffers. */
    for (uint32_t i = 0; i < sk_buf_num; i++)
    {
        sk_mempool->free_indexes[i] = i;

        struct sk_buf *buf = get_sk_buf_from_dma_mem(&sk_mempool->mempool, i);
        buf->mem_pool_idx = i;
        buf->sk_mempool = sk_mempool;
        buf->buf_size = 0;
        buf->phy_addr = virt_to_phy((uint8_t *)&buf);
    }

exit:
    return res;
}

int free_sk_buf(struct sk_buf *sk_buf)
{
    push_free_indexes_stack(sk_buf->sk_mempool, sk_buf->mem_pool_idx);
    return 0;
}

int allocate_sk_buf(struct sk_mempool *sk_mempool, struct sk_buf **sk_buf)
{
    return allocate_sk_buf_batch(sk_mempool, sk_buf, 1);
}

int allocate_sk_buf_batch(struct sk_mempool *sk_mempool,
                          struct sk_buf **sk_buf, uint32_t num)
{
    int res = 0;

    if (sk_mempool->current_index == 0)
    {
        return -ENOBUFS;
    }

    if (sk_mempool->current_index < num)
    {
        log_error("Don't have enough buffers available %d/%d.",
                  sk_mempool->current_index, num);
        num = sk_mempool->current_index;
    }

    for (uint32_t i = 0; i < num; i++)
    {
        uint32_t index = pop_free_indexes_stack(sk_mempool);
        sk_buf[i] = get_sk_buf_from_dma_mem(&sk_mempool->mempool, index);
    }

    res = num;
exit:
    return res;
}