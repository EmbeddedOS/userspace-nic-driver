#include <stdio.h>
#include <fcntl.h>

#include <utility.h>
#include <log.h>
#include <driver/base.h>

static int mmap_2mb_page(int32_t length, uint8_t **addr)
{
    int res = 0;
    int fd = 0;
    char temp_huge_file[40] = {0};

    generate_unique_filepath(temp_huge_file, sizeof(temp_huge_file));
    log_info("Generated huge backed file: %s", temp_huge_file);


    res = open(temp_huge_file, O_CREAT | O_EXCL | O_RDWR, 0777);
}

int main()
{
    log_info("Loading nic driver");

    struct nic_driver *self = nic_driver_init("0000:00:01.0");
    self->send(self, NULL, 0);

    mmap_2mb_page(100, NULL);
}

