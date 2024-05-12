#pragma once

#include <stdint.h>
#include <stddef.h>

extern const unsigned int crc32_table[];

#define CRC32_CALC_CRC(begin, size, initValue) ({ \
    uint32_t crc = initValue; \
    const uint8_t *ptr = (begin); \
    size_t sz = (size); \
    while (sz--) { \
        crc = (crc << 8) ^ crc32_table[((crc >> 24) ^ *ptr) & 255]; \
        ptr++; \
    } \
    crc; \
})
