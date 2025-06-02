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

#define INTEL_82574_RAL0_OFFSET 0x05400
#define INTEL_82574_RAH0_OFFSET 0x05404
#define INTEL_82574_RAL1_OFFSET 0x05408
#define INTEL_82574_RAH1_OFFSET 0x0540C

/* Interrupts */
#define INTEL_82574_IMS_OFFSET 0x000D0 /* Interrupt Mask Set/Read. */
#define INTEL_82574_IMC_OFFSET 0x000D8 /* Interrupt Mask Clear. */

#define INTEL_82574_RCTL_OFFSET 0x00100 /* Receive Control. */

/* Intel 82574 register bits -------------------------------------------------*/
/* Interrupt Mask Set/Read bits. */
#define INTEL_82574_IMS_LCS_BIT 0x02 /* Link status change. */

/* Interrupt Mask Clear bits. */
#define INTEL_82574_IMC_LCS_BIT 0x02 /* Link status change. */

#define INTEL_82574_CTRL0_RST_BIT 0x1A
/* Intel 82574 register masks ------------------------------------------------*/

#define INTEL_82574_CTRL0_RST_MASK (1 << INTEL_82574_CTRL0_RST_BIT)