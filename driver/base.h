#pragma once
#include <stdint.h>
#include <packet.h>

struct nic_driver
{
    uint32_t (*send)(struct nic_driver *drv, const struct skt_buf **buffers, uint32_t len);
    uint32_t (*recv)(struct nic_driver *drv, struct skt_buf **buffers, uint32_t len);
};

struct nic_driver *nic_driver_init(const char *pci_addr);

static inline uint32_t
nic_driver_send(struct nic_driver *drv, const struct skt_buf **buffers, uint32_t len)
{
    return drv->send(drv, buffers, len);
}

int nic_driver_recv(struct nic_driver *drv);