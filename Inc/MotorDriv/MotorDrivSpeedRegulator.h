#pragma once

#include <stdint.h>

void MotorDrivSpeedRegulator_Init(float Kp, float Ki, float Kd, float maxIntegral, float maxInValue, float maxOutValue, uint32_t diffMsTime);
void MotorDrivSpeedRegulator_SetKp(float Kp);
void MotorDrivSpeedRegulator_SetKi(float Ki);
void MotorDrivSpeedRegulator_SetKd(float Kd);
void MotorDrivSpeedRegulator_SetMaxIntegral(float maxIntegral);
void MotorDrivSpeedRegulator_SetMaxInValue(float maxInValue);
void MotorDrivSpeedRegulator_SetMaxOutValue(float maxOutValue);
float MotorDrivSpeedRegulator_Calculate(float targetValue, float actualValue);
