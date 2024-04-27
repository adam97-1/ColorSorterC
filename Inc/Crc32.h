#pragma once

#include <stdint.h>
#include <stddef.h>

uint32_t Crc32_CalcCrc(const uint8_t *begin, size_t size, uint32_t initValue);
