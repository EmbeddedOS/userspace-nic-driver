#pragma once
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

#define set_bit(var, pos) var |= (1 << pos)
#define clr_bit(var, pos) var &= ~(1 << pos)

#define malloc_type(type)                         \
    ({                                            \
        type *tmp = (type *)malloc(sizeof(type)); \
        if (tmp != NULL)                          \
        {                                         \
            memset(tmp, 0, sizeof(type));         \
        }                                         \
        tmp;                                      \
    })

#define CONTAINER_OF(ptr, type, member) ({         \
    const typeof( ((type *)0)->member ) *__mptr = (ptr); \
    (type *)( (char *)__mptr - offsetof(type,member) ); })

int64_t get_filesize(int fd);

char *generate_random_string(char *buf, int len);