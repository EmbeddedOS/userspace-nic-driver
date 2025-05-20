#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include <log.h>
#include <pci.h>

int pci_get_info(const char *pci_addr, struct pci_dev_info *info)
{
    int res = 0;
    int fd = 0;

    res = pci_open(pci_addr, "config", O_RDONLY);
    expr_check_err(res, exit, "pci_open %s failed", pci_addr);
    fd = res;

    res = pci_read16(fd, 0, &info->vendor_id);
    expr_check_err(res, read_failed, "pci_read vendor ID failed");

    res = pci_read16(fd, 2, &info->device_id);
    expr_check_err(res, read_failed, "pci_read device ID failed");

    res = pci_read16(fd, 10, &info->class_id);
    expr_check_err(res, read_failed, "pci_read class ID failed");


    log_info("Device: %s, vendor id: %04X, device id: %04X, class id: %04X",
             pci_addr, info->vendor_id, info->device_id, info->class_id);

read_failed:
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
