#pragma once
#include <stdint.h>
#include <packet.h>

struct nic_stat
{
    uint64_t tx_pkt;
    uint64_t rx_pkt;
    uint64_t tx_byte;
    uint64_t rx_byte;

    /* Error counters. */
    uint64_t crc_err;
    uint64_t alignment_err;
    uint64_t rx_err;
    uint64_t missed_pkt;
};

struct mac_info
{
    uint8_t addr[6];
};

#define log_mac_info(mac)                          \
    log_info("MAC: %02x:%02x:%02x:%02x:%02x:%02x", \
             mac.addr[0],                          \
             mac.addr[1],                          \
             mac.addr[2],                          \
             mac.addr[3],                          \
             mac.addr[4],                          \
             mac.addr[5])

struct nic_driver
{
    uint32_t (*send)(struct nic_driver *drv,
                     const struct sk_buf **buffers, uint32_t len);
    uint32_t (*recv)(struct nic_driver *drv,
                     struct sk_buf **buffers, uint32_t len);
    int (*get_mac_info)(struct nic_driver *drv, struct mac_info *mac_info);
    int (*stat)(struct nic_driver *drv, struct nic_stat *stat);
};

struct nic_driver *nic_driver_init(const char *pci_addr);

static inline uint32_t
nic_driver_send(struct nic_driver *drv,
                const struct sk_buf **buffers, uint32_t len)
{
    return drv->send(drv, buffers, len);
}

static inline int
nic_get_mac_info(struct nic_driver *drv, struct mac_info *mac_info)
{
    return drv->get_mac_info(drv, mac_info);
}

static inline int
nic_read_stat(struct nic_driver *drv, struct nic_stat *stat)
{
    return drv->stat(drv, stat);
}

int nic_driver_recv(struct nic_driver *drv);