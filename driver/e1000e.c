#include <log.h>
#include <utility.h>
#include <io.h>
#include <pci.h>
#include <e1000e.h>
#include <mempool.h>

/* Private function prototypes -----------------------------------------------*/
static int e1000e_sw_reset(struct e1000e_driver *self);

static int e1000e_disable_interrupts(struct e1000e_driver *self);

static int e1000e_enable_interrupts(struct e1000e_driver *self);

static uint32_t e1000e_send(struct nic_driver *drv,
                            const struct packet **buffers, uint32_t len);

static uint32_t e1000e_recv(struct nic_driver *drv,
                            struct packet **buffers, uint32_t len);

/* Private function definitions ----------------------------------------------*/
static uint32_t e1000e_send(struct nic_driver *drv,
                            const struct packet **buffers, uint32_t len)
{
    struct e1000e_driver *self = e1000e_container_of(drv);
    log_info("%s sending...", self->name);

    return 0;
}

static uint32_t e1000e_recv(struct nic_driver *drv,
                            struct packet **buffers, uint32_t len)
{
    return 0;
}

static int e1000e_sw_reset(struct e1000e_driver *self)
{
    int res = 0;

    /* 1. Disable interrupt. */
    res = e1000e_disable_interrupts(self);

    /* 2. Issue global reset and general configuration. */

    /* 3. Disable interrupt again. */
    res = e1000e_disable_interrupts(self);

    /* 4. Setup the PHY and link. */

    /* 5. Initialize statistic counters. */

    /* 6. Initialize receive. */
    mempool_create(10000);

    /* 7. Initialize transmit. */

    /* 5. Enable interrupt. */
    res = e1000e_enable_interrupts(self);

exit:
    return res;
}

static int e1000e_disable_interrupts(struct e1000e_driver *self)
{
    set_reg_bit(self->bar0, INTEL_82574_IMC_OFFSET, INTEL_82574_IMC_LCS_BIT);
    return 0;
}

static int e1000e_enable_interrupts(struct e1000e_driver *self)
{
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

    /* 1. Unbind driver. */
    res = pci_unbind(pci_addr);

    /* 2. Enable DMA. */
    res = pci_enable_bus_mastering(pci_addr);

    /* 3. Map the DMA into process memory. */
    res = pci_mmap(pci_addr, PCI_ATTRIBUTE_RESOURCE0, &self->bar0);

    res = e1000e_sw_reset(self);

    return &self->base;
}
