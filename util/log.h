#pragma once
#include <stdio.h>
#include <stdint.h>
#include <errno.h>

#define log_info(fmt, ...)                           \
    fprintf(stderr, "[INFO ] %s:%d %s(): " fmt "\n", \
            __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define log_error(fmt, ...)                          \
    fprintf(stderr, "[ERROR] %s:%d %s(): " fmt "\n", \
            __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define expr_check_err(result, label, fmt, ...) ({         \
    if (result == -1)                                      \
    {                                                      \
        result = -errno;                                   \
    }                                                      \
    if (result < 0)                                        \
    {                                                      \
        log_error(fmt ", err: %d", ##__VA_ARGS__, result); \
        goto label;                                        \
    }                                                      \
})

static inline void print_reg(const char*name, uint32_t reg)
{
    fprintf(stderr, "Reg %s: ", name);
    for (size_t i = 0; i < 32; i++)
    {
        fprintf(stderr, "%c", (reg & (1u << 31 - i)) ? '1' : '0');
        if (i % 4 == 3)
        {
            fprintf(stderr, " ");
        }
    }
    fprintf(stderr, "\n");
}
