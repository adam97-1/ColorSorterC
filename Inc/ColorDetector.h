#pragma once

#include <stdint.h>
#include <stdbool.h>
typedef enum
  {
    ColorDetector_PinState_High,
    ColorDetector_PinState_Low
  }ColorDetector_PinState;

typedef struct
{
  uint32_t Red;
  uint32_t Green;
  uint32_t Blue;
}ColorDetector_Color;

void ColorDetector_Init();
const ColorDetector_Color* ColorDetector_GetColor();
bool ColorDetector_IsColorReady();
void ColorDetector_SetColorReadyFunc(void (*func)(ColorDetector_Color color));
