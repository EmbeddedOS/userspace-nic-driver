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

struct nic_driver *e1000e_init()
{
    log_info("Loading e1000e driver!");

    struct e1000e_driver *self = malloc_type(struct e1000e_driver);
    self->base.send = &e1000e_send;
    self->base.recv = &e1000e_recv;
    self->name = E1000E_DRIVER_NAME;

    return &self->base;
}
