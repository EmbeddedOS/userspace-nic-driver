/**
 * @brief   - E1000e 82574 GbE nic driver implementation.
 * 
 * @note    - The 82574L MAC layer and PHY layer communicate though an GMII/MII
 *            interface.
 *          - All MAC configuration is performed using device control registers
 *            mapped into system memory or I/O space.
 *          - PHY configuration is operated using an MDIO/MDC inteface, that is
 *            accessible via software. This interface provides the MAC and
 *            software the ability to monitor and control the state of PHY.
 *          - Software can use MDIO accesses to read or write registers in
 *            either GMII or MII mode by accessing the 82574's MDIC register.
 */

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
