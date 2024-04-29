#pragma once

#include <stdint.h>


void MotorDriv_Init(uint32_t msPeriod);
void MotorDriv_SetSpeed(float speed);
float MotorDriv_GetSpeed();
