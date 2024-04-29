#pragma once

#include <stdint.h>

void MotorSel_Init(uint32_t msPeriod);
void MotorSel_SetPosition(float position);
float MotorSel_GetSpeed();
float MotorSel_GetPosition();

