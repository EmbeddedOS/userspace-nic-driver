#pragma once
#include <stdint.h>

#ifndef HUGE_PAGE_MOUNT_POINT
#define HUGE_PAGE_MOUNT_POINT "/mnt/huge"
#endif

int mempool_create(uint32_t length);
