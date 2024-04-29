#pragma once

#include <stdint.h>

void EncoderSel_Init(uint32_t msPeriod, uint32_t maxEncoderValue);
void EncoderSel_SetMaxEncoderValue(uint32_t maxEncoderValue);
float EncoderSel_GetSpeed();
float EncoderSel_GetPosition();
