#include <stdio.h>

#include <log.h>
#include <driver/base.h>

int main()
{
    info("Loading driver");
    nic_driver_init();
}