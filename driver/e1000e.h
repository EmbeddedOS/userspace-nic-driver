#include <base.h>

#define E1000E_DRIVER_NAME "e1000e-userspace-driver"
#define INTEL_CORP_VENDOR_ID 0x8086
#define INTEL_82574L_GIGABIT_DEVICE_ID 0x10d3

struct e1000e_driver
{
    struct nic_driver base;
    const char *name;
};

#define e1000e_container_of(ptr) CONTAINER_OF(ptr, struct e1000e_driver, base)

struct nic_driver* e1000e_init();