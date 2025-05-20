#include <stdint.h>

/* PCI address format [domain]:[bus]:[device]:[function]. */

#define PCI_ADDR_MAX_LEN 20

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

int pci_bind(const char *pci_addr);

int pci_get_binding_state(const char *pci_addr);

#define build_pci_path(attribute, pci_addr, path, len) ({                   \
    snprintf(path, len, "/sys/bus/pci/devices/%s/%s", pci_addr, attribute); \
})

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