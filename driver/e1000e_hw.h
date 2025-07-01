#pragma once

/**
 * @brief - For more detail visit: Intel 82574 GbE Controller Family datasheet.
 */
#define INTEL_CORP_VENDOR_ID 0x8086
#define INTEL_82574L_GIGABIT_DEVICE_ID 0x10d3

/* Intel 82574 register offsets ----------------------------------------------*/
#define INTEL_82574_CTRL0_OFFSET 0x00000
#define INTEL_82574_CTRL1_OFFSET 0x00004
#define INTEL_82574_STATUS_OFFSET 0x00008

#define INTEL_82574_MDIC_OFFSET 0x00020

#define INTEL_82574_RAL0_OFFSET 0x05400
#define INTEL_82574_RAH0_OFFSET 0x05404
#define INTEL_82574_RAL1_OFFSET 0x05408
#define INTEL_82574_RAH1_OFFSET 0x0540C

/* Interrupts. */
#define INTEL_82574_IMS_OFFSET 0x000D0 /* Interrupt Mask Set/Read. */
#define INTEL_82574_IMC_OFFSET 0x000D8 /* Interrupt Mask Clear. */

#define INTEL_82574_RCTL_OFFSET 0x00100 /* Receive Control. */

/* Statistics. */
#define INTEL_82574_CRCERRS_OFFSET 0x04000
#define INTEL_82574_ALGNERRC_OFFSET 0x04004
#define INTEL_82574_RXERRC_OFFSET 0x0400C
#define INTEL_82574_MPC_OFFSET 0x04010

#define INTEL_82574_PRC64_OFFSET 0x0405C /* Packets received 64 bytes count. */
#define INTEL_82574_PRC127_OFFSET 0x04060
#define INTEL_82574_PRC255_OFFSET 0x04064
#define INTEL_82574_PRC511_OFFSET 0x04068
#define INTEL_82574_PRC1023_OFFSET 0x0406C
#define INTEL_82574_PRC1522_OFFSET 0x04070

#define INTEL_82574_GPRC_OFFSET 0x04074 /* Good packet received count. */
#define INTEL_82574_BPRC_OFFSET 0x04078
#define INTEL_82574_MPRC_OFFSET 0x0407C
#define INTEL_82574_GPTC_OFFSET 0x04080 /* Good packet transmitted count. */
#define INTEL_82574_GORCL_OFFSET 0x04088
#define INTEL_82574_GORCH_OFFSET 0x0408C
#define INTEL_82574_GOTCL_OFFSET 0x04090
#define INTEL_82574_GOTCH_OFFSET 0x04094
#define INTEL_82574_TORL_OFFSET 0x040C0 /* Total Octets Received Low. */
#define INTEL_82574_TORH_OFFSET 0x040C4 /* Total Octets Received High. */
#define INTEL_82574_TPR_OFFSET 0x040D0  /* Total packets received. */
#define INTEL_82574_TPT_OFFSET 0x040D4  /* Total packets transmitted. */

#define INTEL_82574_PTC64_OFFSET 0x040D8 /* Packets transmitted bytes count. */
#define INTEL_82574_PTC127_OFFSET 0x040DC
#define INTEL_82574_PTC255_OFFSET 0x040E0
#define INTEL_82574_PTC511_OFFSET 0x040E4
#define INTEL_82574_PTC1023_OFFSET 0x040E8
#define INTEL_82574_PTC1522_OFFSET 0x040EC

/* Intel 82574 PHY register offsets ------------------------------------------*/
#define INTEL_82574_PHY_CTRL_OFFSET 0x00
#define INTEL_82574_PHY_STATUS_OFFSET 0x01
#define INTEL_82574_PHY_IDENTIFIER1_OFFSET 0x02
#define INTEL_82574_PHY_IDENTIFIER2_OFFSET 0x03
#define INTEL_82574_PHY_EXTENDED_STATUS_OFFSET 0x0f

/* Intel 82574 register bits -------------------------------------------------*/
#define INTEL_82574_RCTL_EN_BIT 0x01

/* Interrupt Mask Set/Read bits. */
#define INTEL_82574_IMS_LCS_BIT 0x02 /* Link status change. */

/* Interrupt Mask Clear bits. */
#define INTEL_82574_IMC_LCS_BIT 0x02 /* Link status change. */

#define INTEL_82574_CTRL0_RST_BIT 0x1A
#define INTEL_82574_CTRL0_PHY_RST_BIT 0x1F

#define INTEL_82574_MDIC_DATA_BIT 0x00
#define INTEL_82574_MDIC_REGADD_BIT 0x10
#define INTEL_82574_MDIC_PHYADD_BIT 0x15
#define INTEL_82574_MDIC_OP_BIT 0x1A
#define INTEL_82574_MDIC_R_BIT 0x1C
#define INTEL_82574_MDIC_I_BIT 0x1D
#define INTEL_82574_MDIC_E_BIT 0x1E

/* Intel 82574 register masks ------------------------------------------------*/
#define INTEL_82574_CTRL0_RST_MASK (1 << INTEL_82574_CTRL0_RST_BIT)
#define INTEL_82574_CTRL0_PHY_RST_MASK (1 << INTEL_82574_CTRL0_PHY_RST_BIT)

/* Intel 82574 fields --------------------------------------------------------*/
#define INTEL_82574_MDIC_PHYADD_GIGABIT 0b01
#define INTEL_82574_MDIC_PHYADD_PCIE 0b10

#define INTEL_82574_MDIC_OP_MDI_WRITE 0b01
#define INTEL_82574_MDIC_OP_MDI_READ 0b10

#define INTEL_82574__MRQ_RSS_TYPE_NO_HASH_COMPUTATION 0x0
#define INTEL_82574__MRQ_RSS_TYPE_IPV4_WITH_TCP_HASH 0x1
#define INTEL_82574__MRQ_RSS_TYPE_IPV4_HASH 0x2
#define INTEL_82574__MRQ_RSS_TYPE_IPV6_WITH_TCP_HASH 0x3
#define INTEL_82574__MRQ_RSS_TYPE_IPV6_WITH_EXT_HEADER_HASH 0x4
#define INTEL_82574__MRQ_RSS_TYPE_IPV6_HASH 0x5

/* Intel 82574 fields --------------------------------------------------------*/

#pragma pack(push, 1)

/**
 * @brief   - A receive descriptor is a data structure that contains the receive
 *            data buffer address and fields for hardware to store packet info.
 */

struct e1000e_legacy_rx_desc
{ // Legacy Receive Descriptor Format.
    uint64_t addr;
    uint16_t len;
    uint16_t checksum;
    uint8_t status;
    uint8_t error;
    uint16_t vlan_tags;
};

struct e1000e_extened_rx_desc
{ // Extended Rx Descriptor.
    union
    {
        uint32_t data;
        struct
        {
            uint8_t rss_type : 4;
            uint8_t reserved1 : 4;
            uint8_t queue : 5;
            uint8_t reserved2 : 3;
            uint16_t reserved3;
        } mrq;
    } mrq;

    union
    {
        uint32_t rss_hash;
        struct
        {
            uint16_t ip_identification;
            uint16_t packet_checksum;
        }
    };

    uint32_t extened_status:19;
    uint32_t extended_error:13;
    uint16_t length;
    uint16_t vlan_tag;
};

#pragma pack(pop)
