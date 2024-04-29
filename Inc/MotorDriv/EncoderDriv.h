#pragma once

#include <stdint.h>

void EncoderDriv_Init(uint32_t msPeriod, uint32_t maxEncoderValue);
void EncoderDriv_SetMaxEncoderValue(uint32_t maxEncoderValue);
float EncoderDriv_GetSpeed();
float EncoderDriv_GetPosition();
