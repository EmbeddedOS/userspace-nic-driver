#include <stdio.h>

#include <log.h>
#include <driver/base.h>

int main()
{
    log_info("Loading nic driver");

    struct nic_driver * self = nic_driver_init("0000:00:01.0");
    self->send(self, NULL, 0);
}