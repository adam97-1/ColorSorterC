#pragma once

#include <stdint.h>

void MotorDrivSpeedRegulator_Init(float Kp, float Ki, float Kd, float maxIntegral, float maxSpeed, float maxOut, uint32_t diffMsTime);
void MotorDrivSpeedRegulator_SetKp(float Kp);
void MotorDrivSpeedRegulator_SetKi(float Ki);
void MotorDrivSpeedRegulator_SetKd(float Kd);
void MotorDrivSpeedRegulator_SetMaxIntegral(float maxIntegral);
void MotorDrivSpeedRegulator_SetMaxSpeed(float maxSpeed);
void MotorDrivSpeedRegulator_SetMaxOut(float maxOut);
float MotorDrivSpeedRegulator_SpeedCalculate(float targetSpeed, float actualSpeed);
