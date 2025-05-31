#include <unistd.h>

#include <log.h>
#include <utility.h>
#include <io.h>
#include <pci.h>
#include <e1000e.h>
#include <mempool.h>

#define e1000_write_flush(mem) get_reg(mem, INTEL_82574_STATUS_OFFSET)

/* Private function prototypes -----------------------------------------------*/
static int e1000e_sw_reset(struct e1000e_driver *self);

static int e1000e_phy_init(struct e1000e_driver *self);

static int e1000e_disable_interrupts(struct e1000e_driver *self);

static int e1000e_enable_interrupts(struct e1000e_driver *self);

static int e1000e_get_mac_info(struct nic_driver *drv,
                               struct mac_info *mac_info);

static uint32_t e1000e_send(struct nic_driver *drv,
                            const struct sk_buf **buffers, uint32_t len);

static uint32_t e1000e_recv(struct nic_driver *drv,
                            struct sk_buf **buffers, uint32_t len);

/* Private function definitions ----------------------------------------------*/
static uint32_t e1000e_send(struct nic_driver *drv,
                            const struct sk_buf **buffers, uint32_t len)
{
    struct e1000e_driver *self = e1000e_container_of(drv);
    log_info("%s sending...", self->name);

    return 0;
}

static uint32_t e1000e_recv(struct nic_driver *drv,
                            struct sk_buf **buffers, uint32_t len)
{
    return 0;
}

/**
 * @brief   - Perform software reset the NIC.
 * @brief   - The steps to reset and init the device would be:
 *              1. Disable Interrupts.
 *              2. Issue Global Reset and perform General Configuration.
 *              3. Setup the PHY and the link.
 *              4. Initialize all statistical counters.
 *              5. Initialize Receive.
 *              6. Initialize Transmit.
 *              7. Enable Interrupts.
 * 
 * @ref     - Intel® 82574 GbE Controller Family Datasheet.
 *              Section 4.0 Initialization.
 * @ref     - kernel source: drivers/net/ethernet/intel/e1000/e1000_hw.c
 */
static int e1000e_sw_reset(struct e1000e_driver *self)
{
    int res = 0;

    /* 1. Disable interrupt. */
    res = e1000e_disable_interrupts(self);

    /* 2. Issue global reset and general configuration. */
    log_info("Reseting nic ...");

    /**
     * Must reset the PHY before reset the MAC.
     */
    set_reg_mask(self->bar0, INTEL_82574_CTRL0_OFFSET,
                 INTEL_82574_CTRL0_PHY_RST_MASK);

    /**
     * Software can reset the 82574 by writing the CTRL.RST bit. This bit is
     * self-clearing. This will reset the chip's transmit, receive, DMA and link
     * units. Will not effect the current PCI configuration.
     */
    set_reg_mask(self->bar0, INTEL_82574_CTRL0_OFFSET,
                 INTEL_82574_CTRL0_RST_MASK);
    e1000_write_flush(self->bar0);

    wait_bit_clr(self->bar0, INTEL_82574_CTRL0_OFFSET,
                 INTEL_82574_CTRL0_RST_BIT);
    usleep(10000);

    /* 3. Disable interrupt again. */
    res = e1000e_disable_interrupts(self);

    /* 4. Setup the PHY and link. */
    res = e1000e_phy_init(self);

    /* 5. Initialize statistic counters. */

    /* 6. Initialize receive. */

    /* 7. Initialize transmit. */

    /* 5. Enable interrupt. */
    res = e1000e_enable_interrupts(self);

exit:
    return res;
}

static int e1000e_phy_init(struct e1000e_driver *self)
{

}

static int e1000e_disable_interrupts(struct e1000e_driver *self)
{
    /* Clear interrupt mask to stop board from generating interrupts. */
    log_info("Masking off all interrupts.");
    set_reg(self->bar0, INTEL_82574_IMC_OFFSET, 0xffffffff);
    return 0;
}

static int e1000e_enable_interrupts(struct e1000e_driver *self)
{
    return 0;
}

static int e1000e_get_mac_info(struct nic_driver *drv,
                               struct mac_info *mac_info)
{
    struct e1000e_driver *self = e1000e_container_of(drv);

    uint32_t ral0 = get_reg(self->bar0, INTEL_82574_RAL0_OFFSET);
    uint32_t rah0 = get_reg(self->bar0, INTEL_82574_RAH0_OFFSET);

    mac_info->addr[0] = ral0;
    mac_info->addr[1] = ral0 >> 8;
    mac_info->addr[2] = ral0 >> 16;
    mac_info->addr[3] = ral0 >> 24;
    mac_info->addr[4] = rah0;
    mac_info->addr[5] = rah0 >> 8;

    return 0;
}

/* Public function definitions -----------------------------------------------*/
struct nic_driver *e1000e_init(const char *pci_addr)
{
    int res = 0;
    log_info("Loading e1000e driver!");

    struct e1000e_driver *self = malloc_type(struct e1000e_driver);

    self->name = E1000E_DRIVER_NAME;

    self->base.send = &e1000e_send;
    self->base.recv = &e1000e_recv;
    self->base.get_mac_info = &e1000e_get_mac_info;

    /* 1. Unbind driver. */
    res = pci_unbind(pci_addr);

    /* 2. Enable DMA. */
    res = pci_enable_bus_mastering(pci_addr);

    /* 3. Map the DMA into process memory. */
    res = pci_mmap(pci_addr, PCI_ATTRIBUTE_RESOURCE0, &self->bar0);

    res = e1000e_sw_reset(self);

    return &self->base;
}
