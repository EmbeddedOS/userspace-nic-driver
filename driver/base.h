/**
 * @brief   - Base driver components. Every driver implementations inherit the
 *            nic_driver structure and implement abstracted methods.
 * 
 * @note    - Overview how a nic works:
 *            1. OS sends/receive packets through system memory.
 *            2. Nic's DMA Engine transfer data between ram and nic buffers.
 *            3. Nic's MAC layer handle frame level operations (headers, etc.).
 *            4. Nic's PHY layer translates between digital to physical signals.
 *            5. Signals are sent out a transceiver (cobber, fiber, etc.).
 *
 * @note    - Nic MAC layer operates at OSI L2 that formats frames (Headers,
 *            types, crc), filters (mac address, multicast), flow control, etc.
 *          - Nic Phy Layer operates at OSI L1 that converts digital bits into
 *            electrical, optical, or radio signals and vice versa.
 * @note    - Nic DMA Engine allows nic to read/write packet data directly from/
 *            to system memory without CPU cycle.
 * @note    - Tx/Rx queues and buffers:
 *            - Implemented as ring buffers or descriptors (e.g. e1000e).
 *            - OS and nic shares the descriptors.
 *            - Support for multiple-queue on multiple-core systems.
 * @note    - Interface to Host System:
 *            1. PCIe bus: configuration, info, mapping memory, registers.
 *            2. Interrupt: Notify events (packets received, tx complete).
 *            2. Memory mapping: control registers, descriptors.
 * @note    - MAC-PHY interface standards define how the MAC talks to PHY. Some
 *            interfaces, MII/RMII, GMII, etc.
 */

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