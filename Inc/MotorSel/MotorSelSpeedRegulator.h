#pragma once

#include <stdint.h>

void MotorSelSpeedRegulator_Init(float Kp, float Ki, float Kd, float maxIntegral, float maxInValue, float maxOutValue, uint32_t diffMsTime);
void MotorSelSpeedRegulator_SetKp(float Kp);
void MotorSelSpeedRegulator_SetKi(float Ki);
void MotorSelSpeedRegulator_SetKd(float Kd);
void MotorSelSpeedRegulator_SetMaxIntegral(float maxIntegral);
void MotorSelSpeedRegulator_SetMaxInValue(float maxInValue);
void MotorSelSpeedRegulator_SetMaxOutValue(float maxOutValue);
float MotorSelSpeedRegulator_Calculate(float targetValue, float actualValue);
