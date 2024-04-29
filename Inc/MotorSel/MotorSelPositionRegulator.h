#pragma once

#include <stdint.h>

void MotorSelPositionRegulator_Init(float Kp, float Ki, float Kd, float maxIntegral, float maxInValue, float maxOutValue, uint32_t diffMsTime);
void MotorSelPositionRegulator_SetKp(float Kp);
void MotorSelPositionRegulator_SetKi(float Ki);
void MotorSelPositionRegulator_SetKd(float Kd);
void MotorSelPositionRegulator_SetMaxIntegral(float maxIntegral);
void MotorSelPositionRegulator_SetMaxInValue(float maxInValue);
void MotorSelPositionRegulator_SetMaxOutValue(float maxOutValue);
float MotorSelPositionRegulator_Calculate(float targetValue, float actualValue);
