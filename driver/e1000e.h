#include <base.h>
#include <e1000e_hw.h>

#define E1000E_MAX_RX_QUEUE 1
#define E1000E_MAX_TX_QUEUE 1

#define E1000E_DRIVER_NAME "e1000e-userspace-driver"

/* Public types --------------------------------------------------------------*/
struct e1000e_rx_queue
{
};

struct e1000e_tx_queue
{
};

struct e1000e_driver
{
    struct nic_driver base;
    const char *name;
    uint8_t *bar0;
    struct e1000e_rx_queue rx_queues[E1000E_MAX_RX_QUEUE];
    struct e1000e_tx_queue tx_queues[E1000E_MAX_TX_QUEUE];
};

#define e1000e_container_of(ptr) CONTAINER_OF(ptr, struct e1000e_driver, base)

struct nic_driver *e1000e_init(const char *pci_addr);
