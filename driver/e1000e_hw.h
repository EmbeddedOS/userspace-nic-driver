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

/* Intel 82574 PHY register offsets ------------------------------------------*/
#define INTEL_82574_PHY_CTRL_OFFSET 0x00
#define INTEL_82574_PHY_STATUS_OFFSET 0x01
#define INTEL_82574_PHY_IDENTIFIER1_OFFSET 0x02
#define INTEL_82574_PHY_IDENTIFIER2_OFFSET 0x03
#define INTEL_82574_PHY_EXTENDED_STATUS_OFFSET 0x0f


/* Intel 82574 register bits -------------------------------------------------*/
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
