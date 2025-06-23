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
 * 
 * @note    - Packet reception consists of recognizing the presence of a packet
 *            on the wire, performing address filter, storing the packet in the
 *            receive data FIFO, transferring the data to one of the two receive
 *            queues in host memory, and updating the state of a receive
 *            descriptor.
 *                                    Packets
 *                                      ||
 *                                  ____\/_____
 *                                 |_L2_Filter_|
 *                                      ||
 *                                  ____\/_____
 *                                 |Device_FIFO|
 *                                      ||
 *                         Select Queue in host memory
 *                                      ||
 *                                //====\/====\\
 *                            ___//__        __\\___
 *                           |Queue_1|      |Queue_2|
 *
 *          1. Packet address filter.
 *          2. 
 * 
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

/**
 * @brief   - e1000e nic driver structure.
 *          - The nic device is connected to PCIe bus and expose 4 BARs.
 *            1. Memory BAR0: internal registers and memories.
 *            2. Flash BAR1: External flash.
 *            3. I/O BAR2.
 *            4. MSI-X BAR3.
 * @note    - To access BARs remember to enable PCI Memory Access Enable bit
 *            first.
 * @note    - We only map BAR0 to our application to read/write nic registers
 *            and memories.
 */
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
