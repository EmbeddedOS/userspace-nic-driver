#include <unistd.h>

#include <log.h>
#include <utility.h>
#include <io.h>
#include <pci.h>
#include <e1000e.h>
#include <mempool.h>

#define e1000_write_flush(mem) get_reg(mem, INTEL_82574_STATUS_OFFSET)

#define e1000e_get_mdic_data(mdic) (0x0000ffff & mdic)

/* Private function prototypes -----------------------------------------------*/
static int e1000e_sw_reset(struct e1000e_driver *self);

static int e1000e_phy_read_reg(struct e1000e_driver *self, uint8_t phy_reg,
                               uint16_t *val);

static int e1000e_phy_write_reg(struct e1000e_driver *self, uint8_t phy_reg,
                                uint16_t val);

static int e1000e_phy_init(struct e1000e_driver *self);

static int e1000e_init_stat_counters(struct e1000e_driver *self);

static int e1000e_init_rx(struct e1000e_driver *self);

static int e1000e_init_tx(struct e1000e_driver *self);

static int e1000e_start_rx_queue(struct e1000e_driver *self);

static int e1000e_disable_interrupts(struct e1000e_driver *self);

static int e1000e_enable_interrupts(struct e1000e_driver *self);

static int e1000e_get_mac_info(struct nic_driver *drv,
                               struct mac_info *mac_info);

static uint32_t e1000e_send(struct nic_driver *drv,
                            const struct sk_buf **buffers, uint32_t len);

static uint32_t e1000e_recv(struct nic_driver *drv,
                            struct sk_buf **buffers, uint32_t len);

static int e1000e_stat(struct nic_driver *drv, struct nic_stat *stat);

/* Private function definitions ----------------------------------------------*/

/**
 * @brief   - Read a PHY register via MDIC register.
 * @note    - Reading steps:
 *
 * @ref     - Intel® 82574 GbE Controller Family Datasheet: 10.2.2.7 MDI Control
 *            Register - MDIC.
 */
static int e1000e_phy_read_reg(struct e1000e_driver *self, uint8_t phy_reg,
                               uint16_t *val)
{
    int res = 0;
    uint32_t mdic = 0;

    mdic = 0;
    mdic |= (phy_reg << INTEL_82574_MDIC_REGADD_BIT);
    mdic |= (INTEL_82574_MDIC_PHYADD_GIGABIT << INTEL_82574_MDIC_PHYADD_BIT);
    mdic |= (INTEL_82574_MDIC_OP_MDI_READ << INTEL_82574_MDIC_OP_BIT);
    clr_bit(mdic, INTEL_82574_MDIC_R_BIT);

    set_reg(self->bar0, INTEL_82574_MDIC_OFFSET, mdic);
    e1000_write_flush(self->bar0);
    usleep(10000);

    // Wait until read flag become ready.
    wait_bit_set(self->bar0, INTEL_82574_MDIC_OFFSET, INTEL_82574_MDIC_R_BIT);
    mdic = get_reg(self->bar0, INTEL_82574_MDIC_OFFSET);

    if (get_bit(mdic, INTEL_82574_MDIC_E_BIT))
    { // Check error flag.
        res = e1000e_get_mdic_data(mdic);
    }
    else
    {
        *val = e1000e_get_mdic_data(mdic);
    }

exit:
    return res;
}

static int e1000e_phy_write_reg(struct e1000e_driver *self, uint8_t phy_reg,
                                uint16_t val)
{
    int res = 0;
    uint32_t mdic = 0;

    mdic = val;
    mdic |= (phy_reg << INTEL_82574_MDIC_REGADD_BIT);
    mdic |= (INTEL_82574_MDIC_PHYADD_GIGABIT << INTEL_82574_MDIC_PHYADD_BIT);
    mdic |= (INTEL_82574_MDIC_OP_MDI_WRITE << INTEL_82574_MDIC_OP_BIT);
    clr_bit(mdic, INTEL_82574_MDIC_R_BIT);

    set_reg(self->bar0, INTEL_82574_MDIC_OFFSET, mdic);
    e1000_write_flush(self->bar0);
    usleep(10000);

    // Wait until read flag become ready.
    wait_bit_set(self->bar0, INTEL_82574_MDIC_OFFSET, INTEL_82574_MDIC_R_BIT);
    mdic = get_reg(self->bar0, INTEL_82574_MDIC_OFFSET);

    if (get_bit(mdic, INTEL_82574_MDIC_E_BIT))
    { // Check error flag.
        res = e1000e_get_mdic_data(mdic);
    }

exit:
    return res;
}

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
    struct e1000e_driver *self = e1000e_container_of(drv);


    return 0;
}

/**
 * @brief   - Stat NIC counter by reading statistic registers. These registers
 *            are clear-on-read, so we need to manage these counters in
 *            software.
 */
static int e1000e_stat(struct nic_driver *drv, struct nic_stat *stat)
{
    int res = 0;
    struct e1000e_driver *self = e1000e_container_of(drv);

    uint32_t crc_err = get_reg(self->bar0, INTEL_82574_CRCERRS_OFFSET);
    uint32_t align_err = get_reg(self->bar0, INTEL_82574_ALGNERRC_OFFSET);
    uint32_t rx_err = get_reg(self->bar0, INTEL_82574_RXERRC_OFFSET);
    uint32_t mpc_err = get_reg(self->bar0, INTEL_82574_MPC_OFFSET);

    uint32_t good_pkt_recv = get_reg(self->bar0, INTEL_82574_GPRC_OFFSET);
    uint32_t good_pkt_tran = get_reg(self->bar0, INTEL_82574_GPTC_OFFSET);

    uint64_t rx_bytes = get_reg(self->bar0, INTEL_82574_GORCL_OFFSET) +
                        ((uint64_t)get_reg(self->bar0, INTEL_82574_GORCH_OFFSET)
                         << 32);

    uint64_t tx_bytes = get_reg(self->bar0, INTEL_82574_GOTCL_OFFSET) +
                        ((uint64_t)get_reg(self->bar0, INTEL_82574_GOTCH_OFFSET)
                         << 32);

    stat->alignment_err += align_err;
    stat->crc_err += crc_err;
    stat->missed_pkt += mpc_err;
    stat->rx_err += rx_err;
    stat->rx_pkt += good_pkt_recv;
    stat->tx_pkt += good_pkt_tran;
    stat->rx_byte += rx_bytes;
    stat->tx_byte += tx_bytes;
exit:
    return res;
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
    set_reg_bit(self->bar0, INTEL_82574_CTRL0_OFFSET,
                INTEL_82574_CTRL0_PHY_RST_BIT);

    /**
     * Software can reset the 82574 by writing the CTRL.RST bit. This bit is
     * self-clearing. This will reset the chip's transmit, receive, DMA and link
     * units. Will not effect the current PCI configuration.
     */
    set_reg_bit(self->bar0, INTEL_82574_CTRL0_OFFSET,
                INTEL_82574_CTRL0_RST_BIT);
    e1000_write_flush(self->bar0);

    wait_bit_clr(self->bar0, INTEL_82574_CTRL0_OFFSET,
                 INTEL_82574_CTRL0_RST_BIT);
    usleep(10000);

    /* 3. Disable interrupt again. */
    res = e1000e_disable_interrupts(self);

    /* 4. Setup the PHY and link. */
    res = e1000e_phy_init(self);

    /* 5. Initialize statistic counters. */
    res = e1000e_init_stat_counters(self);

    /* 6. Initialize receive. */
    res = e1000e_init_rx(self);

    /* 7. Initialize transmit. */
    res = e1000e_init_tx(self);

    res= e1000e_start_rx_queue(self);

    /* 5. Enable interrupt. */
    res = e1000e_enable_interrupts(self);

exit:
    return res;
}

/**
 * @brief   - Initialize the 82574 PHY layer.
 *          - The device driver uses the MDIC register to initialize the PHY and
 *            setup the link.
 */
static int e1000e_phy_init(struct e1000e_driver *self)
{
    int res = 0;
    uint32_t phy_status = 0;

    log_info("Setting up PHY and link.");

    res = e1000e_phy_read_reg(self, INTEL_82574_PHY_STATUS_OFFSET,
                              (uint16_t *)&phy_status);

    print_reg("PHY STATUS", phy_status);

    return res;
}

static int e1000e_init_stat_counters(struct e1000e_driver *self)
{
    struct nic_stat nic_stat = {0};

    /* Statistic counter registers are clear-on-read, so we read to clear them
     * and reset the value to zero. */
    e1000e_stat(&self->base, &nic_stat);

    return 0;
}


/**
 * @brief   - Initialize Rx. The following should be done once per rx queue:
 *            1. Allocate a region of memory for the receive descriptor lists.
 *            2. Receive buffers of appropriate size should be allocated and
 *               pointers to these buffers should be stored in the descriptor
 *               ring.
 *            3. Program the descriptor base address with the address of the
 *               region.
 *            4. Set the length register to the size of the descriptor ring.
 *            5. Program the head and tail registers. The tail should be set to
 *               point one descriptor beyond the end.
 * 
 * @note    - It's better to leave the receive logic disable RCTL.EN = 0b until
 *            the receive descriptor ring has been initialized.
 *
 * @ref     - Intel® 82574 GbE Controller Family Datasheet.
 *              Section 4.6.5 Receive Initialization.
 */
static int e1000e_init_rx(struct e1000e_driver *self)
{
    int res = 0;

    /* 1. Disable rx temporarily while configuring. */
    clr_reg_bit(self->bar0, INTEL_82574_RCTL_OFFSET, INTEL_82574_RCTL_EN_BIT);

    /* 2. Config buffer packet size, we set 8192 bytes. */
    set_reg_bit(self->bar0, INTEL_82574_RCTL_OFFSET, INTEL_82574_RCTL_BSEX_BIT);
    set_reg_bit_range(self->bar0, INTEL_82574_RCTL_OFFSET,
                      INTEL_82574_RCTL_BSIZE_BIT,
                      2, INTEL_82574_RCTL_BSIZE_8192);

    /* 3. Enable checksum offloading. */
    set_reg_bit(self->bar0, INTEL_82574_RCTL_OFFSET,
                INTEL_82574_RCTL_SECRC_BIT);

    /* 4. Accept broadcast packets. */
    set_reg_bit(self->bar0, INTEL_82574_RCTL_OFFSET, INTEL_82574_RCTL_BAM_BIT);

    /* 5. Configure rx queues. */
    for (uint8_t i = 0; i < INTEL_82574_MAX_HOST_RX_QUEUE; i++)
    {
        struct mem mem = {0};
        uint32_t ring_size = E1000E_RECV_DESCRIPTOR_ENTRIES *
                             sizeof(union e1000e_extended_rx_desc);

        /* Enable extended Rx descriptor. */
        set_reg_bit(self->bar0, INTEL_82574_RFCTL_OFFSET,
                    INTEL_82574_RFCTL_EXSTEN_BIT);
        set_reg_bit_range(self->bar0, INTEL_82574_RCTL_OFFSET,
                          INTEL_82574_RCTL_DTYP_BIT, 2,
                          INTEL_82574_RCTL_DTYP_LEGACY_DESC_TYPE);

        /* We allocate a huge memory to map the circular ring buffer receive
         * descriptors into memory. */
        res = allocate_huge_page(ring_size, &mem);
        expr_check_err(res, exit, "allocate_huge_page failed");

        /* Prevent rogue memory accesses on premature DMA activation. */
        memset(mem.virt, 0xFF, ring_size);

        /* Configure descriptor base address. */
        set_reg(self->bar0, INTEL_82574_RDBAL0_OFFSET(i),
                (uint32_t)(mem.phy & 0xFFFFFFFFull));
        set_reg(self->bar0, INTEL_82574_RDBAH0_OFFSET(i),
                (uint32_t)(mem.phy >> 32));

        /* Configure descriptor ring size. */
        set_reg(self->bar0, INTEL_82574_RDLEN0_OFFSET(i), ring_size);

        /* Reset descriptor ring head and tail. */
        set_reg(self->bar0, INTEL_82574_RDH0_OFFSET(i), 0);
        set_reg(self->bar0, INTEL_82574_RDT0_OFFSET(i),
                E1000E_RECV_DESCRIPTOR_ENTRIES - 1);

        self->rx_queues[i].recv_desc_ring =
            (union e1000e_extended_rx_desc *)mem.virt;

        /* Allocate a mempool for socket buffers. */
        res = allocate_sk_mempool(&self->rx_queues[i].pkt_buffer_pool,
                                  E1000E_SOCKET_BUFFER_NUMS,
                                  E1000E_SOCKET_BUFFER_SIZE);

        expr_check_err(res, exit, "allocate_sk_mempool failed");

        for (uint16_t j = 0; j < E1000E_SOCKET_BUFFER_NUMS; j++)
        {
            union e1000e_extended_rx_desc *desc = 
                self->rx_queues[i].recv_desc_ring + j;

            struct sk_buf *sk_buf = NULL;
            res = allocate_sk_buf(&self->rx_queues[i].pkt_buffer_pool, &sk_buf);

            /* Descriptor hold the buffer address. */
            desc->buffer.addr = sk_buf->phy_addr + offsetof(struct sk_buf, buf);

            /* Cache buffer virtual address to easy access by index. */
            self->rx_queues[i].pkt_buffer_virt_addrs[j] = sk_buf;
        }
    }

exit:
    /* TODO: Clean up allocated memories in case of failure. */

    /* 6. Enable rx. */
    set_reg_bit(self->bar0, INTEL_82574_RCTL_OFFSET, INTEL_82574_RCTL_EN_BIT);
    return res;
}

static int e1000e_init_tx(struct e1000e_driver *self)
{
    return 0;
}

static int e1000e_start_rx_queue(struct e1000e_driver *self)
{
    int res = 0;

    for (int i = 0; i < INTEL_82574_MAX_HOST_RX_QUEUE; i++)
    {
        set_reg(self->bar0, INTEL_82574_RDH0_OFFSET(i), 0);

    }

    return res;
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
    self->base.stat = &e1000e_stat;

    /* 1. Unbind driver. */
    res = pci_unbind(pci_addr);

    /* 2. Enable DMA. */
    res = pci_enable_bus_mastering(pci_addr);

    /* 3. Map the bar0 into process memory. */
    res = pci_mmap(pci_addr, PCI_ATTRIBUTE_RESOURCE0, &self->bar0);

    res = e1000e_sw_reset(self);

    return &self->base;
}
