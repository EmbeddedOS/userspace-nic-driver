#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <log.h>
#include <utility.h>
#include <pci.h>

int pci_get_info(const char *pci_addr, struct pci_dev_info *info)
{
    int res = 0;
    int fd = 0;

    res = pci_open(pci_addr, PCI_ATTRIBUTE_CONFIG, O_RDONLY);
    expr_check_err(res, exit, "pci_open %s failed", pci_addr);
    fd = res;

    res = pci_read16(fd, PCI_CONFIG_VENDOR_ID_OFFSET, &info->vendor_id);
    expr_check_err(res, read_failed, "pci_read vendor ID failed");

    res = pci_read16(fd, PCI_CONFIG_DEVICE_ID_OFFSET, &info->device_id);
    expr_check_err(res, read_failed, "pci_read device ID failed");

    res = pci_read16(fd, PCI_CONFIG_CLASS_CODE_OFFSET, &info->class_id);
    expr_check_err(res, read_failed, "pci_read class ID failed");

    log_info("Device: %s, vendor id: %04X, device id: %04X, class id: %04X",
             pci_addr, info->vendor_id, info->device_id, info->class_id);

read_failed:
    pci_close(fd);
exit:
    return res;
}

int pci_unbind(const char *pci_addr)
{
    int res = 0;
    int fd = 0;

    res = pci_open(pci_addr, PCI_ATTRIBUTE_UNBIND, O_WRONLY);
    expr_check_err(res, exit, "pci_open %s failed", pci_addr);
    fd = res;

    res = pci_write(fd, 0, pci_addr, strlen(pci_addr));
    expr_check_err(res, write_failed, "pci_write %s failed", pci_addr);

write_failed:
    pci_close(fd);

exit:
    return res;
}

/**
 * @brief   - Enable bus mastering, allows PCI adapter to directly access system
 *            memory without CPU's intervention. We do that by set bit 2 of the
 *            command register in the PCI configuration space.
 */
int pci_enable_bus_mastering(const char *pci_addr)
{
    int res = 0;
    int fd = 0;
    uint16_t pci_config_command = 0;
    res = pci_open(pci_addr, PCI_ATTRIBUTE_CONFIG, O_RDWR);
    expr_check_err(res, exit, "pci_open %s failed", pci_addr);
    fd = res;

    res = pci_read16(fd, PCI_CONFIG_COMMAND_OFFSET, &pci_config_command);
    expr_check_err(res, read_failed, "pci_read %s failed", pci_addr);

    set_bit(pci_config_command, PCI_CONFIG_COMMAND_BUS_MASTER_ENABLE_BIT);

    res = pci_write16(fd, PCI_CONFIG_COMMAND_OFFSET, &pci_config_command);
    expr_check_err(res, write_failed, "pci_write %s failed", pci_addr);

write_failed:
read_failed:
    pci_close(fd);

exit:
    return res;
}

int pci_mmap(const char *pci_addr, const char *resource, uint8_t **ptr)
{
    int res = 0;
    int fd = 0;
    int64_t resource_len = 0;

    res = pci_open(pci_addr, resource, O_RDWR);
    expr_check_err(res, exit, "pci_open %s/%s failed", pci_addr, resource);
    fd = res;

    resource_len = get_filesize(fd);
    expr_check_err(resource_len, get_filesize_failed,
                   "Get file size %s/%s failed", pci_addr, resource);

    *ptr = (uint8_t *)mmap(NULL, stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (*ptr == MAP_FAILED)
    {
        res = -errno;
        log_error("Failed to map resource: %d", res);
        goto mmap_failed;
    }

mmap_failed:
get_filesize_failed:
    pci_close(fd);

exit:
    return res;
}

int pci_open(const char *pci_addr, const char *attribute, int flags)
{
    int fd = 0;
    char attr_path[100] = {0};

    build_pci_path(attribute, pci_addr, attr_path, sizeof(attr_path));

    fd = open(attr_path, flags);
    expr_check_err(fd, exit, "open %s failed", attr_path);

exit:
    return fd;
}

int pci_read(int fd, int pos, uint8_t *buf, int len)
{
    int res = pread(fd, buf, len, pos);
    expr_check_err(res, exit, "pread %d failed", fd);
    if (res != len)
    {
        log_error("Reading is not completed %d/%d.", res, len);
    }

exit:
    return res;
}

int pci_write(int fd, int pos, const uint8_t *buf, int len)
{
    int res = pwrite(fd, buf, len, pos);
    expr_check_err(res, exit, "pwrite %d failed", fd);
    if (res != len)
    {
        log_error("Writing is not completed %d/%d.", res, len);
    }

exit:
    return res;
}

int pci_close(int fd)
{
    return close(fd);
}
