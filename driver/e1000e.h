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
 * @note    - Receive functionality.
 *          - Packet reception consists of:
 *            1. Recognizing the presence of a packet on the wire.
 *            2. Performing address filtering.
 *            3. Storing the packet in the receive data FIFO.
 *            4. Transferring the data to assigned receive queues (one of the
 *               two receive queues) in host memory.
 *            5. Updating the state of a receive descriptor.
 *
 *          - Receive descriptor:
 *          - A receive descriptor is data structure that is used by software to
 *            tell hardware where to place incoming network packets (provide
 *            buffer information such as address, length, etc.). The Hardware
 *            and then write-back incoming packet to that memory.
 *            1. When a packet arrives the hardware read the next available
 *               descriptor (using head pointer) to write the packet data into
 *               that buffer.
 *            2. After writing the data, hardware update the descriptor to
 *               indicate that the buffer was filled and ready to process. The
 *               hardware can raise an interrupt for this.
 *            3. Driver processes the packet, normally move to other location.
 *            4. Driver reset the descriptor state.
 */

#include <base.h>
#include <e1000e_hw.h>

#define E1000E_MAX_TX_QUEUE 2

#define E1000E_DRIVER_NAME "e1000e-userspace-driver"
#define E1000E_RECV_DESCRIPTOR_ENTRIES 1028
#define E1000E_SOCKET_BUFFER_NUMS E1000E_RECV_DESCRIPTOR_ENTRIES
#define E1000E_SOCKET_BUFFER_SIZE 2048
/* Public types --------------------------------------------------------------*/
struct e1000e_rx_queue
{
    volatile union e1000e_extended_rx_desc *recv_desc_ring;
    struct sk_mempool pkt_buffer_pool;
    void *pkt_buffer_virt_addrs[E1000E_SOCKET_BUFFER_NUMS];

    uint32_t idx;
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
    struct e1000e_rx_queue rx_queues[INTEL_82574_MAX_HOST_RX_QUEUE];
    struct e1000e_tx_queue tx_queues[E1000E_MAX_TX_QUEUE];
};

#define e1000e_container_of(ptr) CONTAINER_OF(ptr, struct e1000e_driver, base)

struct nic_driver *e1000e_init(const char *pci_addr);
