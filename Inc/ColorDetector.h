#pragma once

#include <stdint.h>

typedef struct
{
  uint32_t Red;
  uint32_t Green;
  uint32_t Blue;
}Color;

void ColorDetector_Init();
