#include <base.h>

#define E1000E_MAX_RX_QUEUE 1
#define E1000E_MAX_TX_QUEUE 1

#define E1000E_DRIVER_NAME "e1000e-userspace-driver"

/**
 * @brief - For more detail visit: Intel 82574 GbE Controller Family datasheet.
 */
#define INTEL_CORP_VENDOR_ID 0x8086
#define INTEL_82574L_GIGABIT_DEVICE_ID 0x10d3

/* Intel 82574 register offsets ----------------------------------------------*/
#define INTEL_82574_BAR0_CTRL0_OFFSET 0x00000
#define INTEL_82574_BAR0_CTRL1_OFFSET 0x00004
#define INTEL_82574_BAR0_STATUS_OFFSET 0x00008

/* Interrupts */
#define INTEL_82574_IMS_OFFSET 0x000D0 /* Interrupt Mask Set/Read. */
#define INTEL_82574_IMC_OFFSET 0x000D8 /* Interrupt Mask Clear. */

#define INTEL_82574_RCTL_OFFSET 0x00100 /* Receive Control. */

/* Intel 82574 register bits -------------------------------------------------*/
/* Interrupt Mask Set/Read bits. */
#define INTEL_82574_IMS_LCS_BIT 0x02 /* Link status change. */

/* Interrupt Mask Clear bits. */
#define INTEL_82574_IMC_LCS_BIT 0x02 /* Link status change. */


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
