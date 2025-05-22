#include <log.h>
#include <utility.h>

#include <pci.h>

#include <e1000e.h>

static uint32_t e1000e_send(struct nic_driver *drv, const struct packet **buffers, uint32_t len);

static uint32_t e1000e_recv(struct nic_driver *drv, struct packet **buffers, uint32_t len);

static uint32_t e1000e_send(struct nic_driver *drv, const struct packet **buffers, uint32_t len)
{
    struct e1000e_driver *self = e1000e_container_of(drv);
    log_info("%s sending...", self->name);

    return 0;
}

static uint32_t e1000e_recv(struct nic_driver *drv, struct packet **buffers, uint32_t len)
{
    return 0;
}

struct nic_driver *e1000e_init(const char *pci_addr)
{
    int res = 0;
    log_info("Loading e1000e driver!");

    struct e1000e_driver *self = malloc_type(struct e1000e_driver);

    self->name = E1000E_DRIVER_NAME;

    self->base.send = &e1000e_send;
    self->base.recv = &e1000e_recv;

    /* 1. Unbind driver. */
    pci_unbind(pci_addr);

    /* 2. Enable DMA. */
    pci_enable_bus_mastering(pci_addr);

    /* 3. Map the DMA into process memory. */
    pci_mmap(pci_addr, "resource0", &self->bar0);

    /* Now we have device memory on user space. */

    return &self->base;
}
