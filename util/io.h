#pragma once
#include <stdint.h>

#define get_bit(reg, pos) (reg & (1 << pos))

static inline void set_reg(uint8_t *base, uint32_t offset, uint32_t val)
{
    *((volatile uint32_t *)(base + offset)) = val;
}

static inline uint32_t get_reg(const uint8_t *base, uint32_t offset)
{
    return *((volatile uint32_t *)(base + offset));
}

static inline void set_reg_bit(uint8_t *base, int offset, uint8_t pos)
{
    set_reg(base, offset, get_reg(base, offset) | (1 << pos));
}

static inline void clr_reg_bit(uint8_t *base, int offset, uint8_t pos)
{
    set_reg(base, offset, get_reg(base, offset) & ~(1 << pos));
}

static inline void wait_bit(const uint8_t *base, int offset, uint8_t pos,
                            uint8_t val)
{
    uint32_t reg = 0;
    do
    {
        reg = get_reg(base, offset);
    } while (get_bit(reg, pos) == val);
}

#define wait_bit_set(base, offset, pos) wait_bit(base, offset, pos, 1)
#define wait_bit_clr(base, offset, pos) wait_bit(base, offset, pos, 0)
