#include <stdio.h>
#include <fcntl.h>

#include <utility.h>
#include <log.h>
#include <driver/base.h>

int main()
{
    struct mac_info mac_info = {0};
    log_info("Loading nic driver");

    struct nic_driver *self = nic_driver_init("0000:00:01.0");
    self->send(self, NULL, 0);

    self->get_mac_info(self, &mac_info);
    log_mac_info(mac_info);
}

