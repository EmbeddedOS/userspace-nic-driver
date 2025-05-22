#include <stdint.h>

/* PCI Attributes ------------------------------------------------------------*/
#define PCI_ATTRIBUTE_CONFIG "config"
#define PCI_ATTRIBUTE_UNBIND "driver/unbind"
#define PCI_ATTRIBUTE_RESOURCE0 "resource0"

/* PCI Common Configuration Space Header -------------------------------------*/
#define PCI_CONFIG_VENDOR_ID_OFFSET 0x00
#define PCI_CONFIG_DEVICE_ID_OFFSET 0x02
#define PCI_CONFIG_COMMAND_OFFSET 0x04
#define PCI_CONFIG_STATUS_OFFSET 0x06
#define PCI_CONFIG_REVISION_ID_OFFSET 0x08
#define PCI_CONFIG_CLASS_CODE_OFFSET (0x09 + 1)
#define PCI_CONFIG_CACHE_LINE_SIZE_OFFSET 0x0C
#define PCI_CONFIG_MASTER_LATENCY_TIMER_OFFSET 0x0D
#define PCI_CONFIG_HEADER_TYPE_OFFSET 0x0E
#define PCI_CONFIG_BIST_OFFSET 0x0F
#define PCI_CONFIG_HEADER_TYPE_SPECIFIC_OFFSET 0x10
#define PCI_CONFIG_CAPABILITIES_POINTER_OFFSET 0x34
#define PCI_CONFIG_INTERRUPT_LINE_OFFSET 0x3C
#define PCI_CONFIG_INTERRUPT_PIN_OFFSET 0x3D

#define PCI_CONFIG_COMMAND_BUS_MASTER_ENABLE_BIT 0x02

/* PCI address format [domain]:[bus]:[device]:[function]. */
#define PCI_ADDR_MAX_LEN (sizeof("0000:00:00.0") + 1)

struct pci_dev_info
{
    uint16_t domain_number;
    uint16_t bus_number;
    uint16_t device_number;
    uint16_t function_number;

    uint16_t device_id;
    uint16_t vendor_id;
    uint16_t class_id;
    uint16_t subsystem_device_id;
    uint16_t subsystem_vendor_id;
};

int pci_get_info(const char *pci_addr, struct pci_dev_info *info);

int pci_unbind(const char *pci_addr);

int pci_enable_bus_mastering(const char *pci_addr);

#define build_pci_path(attribute, pci_addr, path, len) ({                   \
    snprintf(path, len, "/sys/bus/pci/devices/%s/%s", pci_addr, attribute); \
})

int pci_mmap(const char *pci_addr, const char *resource, uint8_t **ptr);

/* PCI sysfs file operations -------------------------------------------------*/
int pci_open(const char *pci_addr, const char *attribute, int flags);

int pci_read(int fd, int pos, uint8_t *buf, int len);

int pci_write(int fd, int pos, const uint8_t *buf, int len);

int pci_close(int fd);

static inline int pci_read32(int fd, int pos, uint32_t *buf)
{
    return pci_read(fd, pos, (uint8_t *)buf, 4);
}

static inline int pci_read16(int fd, int pos, uint16_t *buf)
{
    return pci_read(fd, pos, (uint8_t *)buf, 2);
}

static inline int pci_write32(int fd, int pos, const uint32_t *buf)
{
    return pci_write(fd, pos, (const uint8_t *)buf, 4);
}

static inline int pci_write16(int fd, int pos, const uint16_t *buf)
{
    return pci_write(fd, pos, (const uint8_t *)buf, 2);
}