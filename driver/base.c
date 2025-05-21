#include <stdlib.h>

#include <log.h>
#include <pci.h>
#include <e1000e.h>

struct nic_driver *nic_driver_init(const char *pci_addr)
{
    struct pci_dev_info pci_info = {0};
    int res = 0;

    res = pci_get_info(pci_addr, &pci_info);
    expr_check_err(res, exit, "pci_get_info failed");

    if (pci_info.device_id == INTEL_82574L_GIGABIT_DEVICE_ID && 
        pci_info.vendor_id == INTEL_CORP_VENDOR_ID)
    {
        return e1000e_init(pci_addr);
    }
    else
    {
        log_error("Can't find driver!");
    }

exit:
    return NULL;
}
