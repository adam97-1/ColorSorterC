#include "MotorDriv/MotorDrivSpeedRegulator.h"

#include "math.h"
#include "ClearStack.h"

static float m_Kp = 1;
static float m_Ki = 0;
static float m_Kd = 0;
static float m_maxIntegral = 10000000;
static float m_maxInValue = 1000000;
static float m_maxOutValue = 1000000;
static uint32_t m_diffMsTime = 1;

static float m_errorIntegral = 0;
static float m_oldValueError = 0;

void MotorDrivSpeedRegulator_Init(	float Kp, float Ki, float Kd,
									float maxIntegral, float maxInValue, float maxOutValue,
									uint32_t diffMsTime)
{
	ADD_TO_CLEAR();
	m_Kp = Kp;
	m_Ki = Ki;
	m_Kd = Kd;
	m_maxIntegral = maxIntegral;
	m_maxInValue = maxInValue;
	m_maxOutValue = maxOutValue;
	m_diffMsTime = diffMsTime;
}

void MotorDrivSpeedRegulator_SetKp(float Kp)
{
	ADD_TO_CLEAR();
	m_Kp = Kp;
}

void MotorDrivSpeedRegulator_SetKi(float Ki)
{
	ADD_TO_CLEAR();
	m_Ki = Ki;
}


void MotorDrivSpeedRegulator_SetKd(float Kd)
{
	ADD_TO_CLEAR();
	m_Kd = Kd;
}

void MotorDrivSpeedRegulator_SetMaxIntegral(float maxIntegral)
{
	ADD_TO_CLEAR();
	m_maxIntegral = maxIntegral;
}

void MotorDrivSpeedRegulator_SetMaxInValue(float maxInValue)
{
	ADD_TO_CLEAR();
	m_maxInValue = maxInValue;
}

void MotorDrivSpeedRegulator_SetMaxOutValue(float maxOutValue)
{
	ADD_TO_CLEAR();
	m_maxOutValue = maxOutValue;
}

static inline float Saturation(float value, float maxValue)
{
	ADD_TO_CLEAR();
	if (value > maxValue)
		value = maxValue;
	else if (value < -maxValue)
		value = -maxValue;
	return value;
}

float MotorDrivSpeedRegulator_Calculate(float targetValue, float actualValue)
{
	ADD_TO_CLEAR();
	targetValue = Saturation(targetValue, m_maxInValue);
	CLEAR_STACK();
	float errorValue = targetValue - actualValue;

	m_errorIntegral += errorValue * (m_diffMsTime * 0.001f);
	m_errorIntegral = Saturation(m_errorIntegral, m_maxIntegral);
	CLEAR_STACK();

	float errorDiffValue = (m_oldValueError - errorValue)
			/ (m_diffMsTime * 0.001f);
	m_oldValueError = errorValue;

	float out = (m_Kp * errorValue + m_Ki * m_errorIntegral	+ m_Kd * errorDiffValue);
	out = Saturation(out, m_maxOutValue);
	CLEAR_STACK();

	if (fabs(out) >= m_maxOutValue)
		m_errorIntegral -= errorValue * (m_diffMsTime * 0.001f);

	return out;
}
