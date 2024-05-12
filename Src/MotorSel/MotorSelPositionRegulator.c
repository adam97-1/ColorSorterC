#include "MotorSel/MotorSelPositionRegulator.h"

#include "math.h"
#include "Functions.h"

static float m_Kp = 1;
static float m_Ki = 0;
static float m_Kd = 0;
static float m_maxIntegral = 10000000;
static float m_maxInValue = 1000000;
static float m_maxOutValue = 1000000;
static uint32_t m_diffMsTime = 1;

static float m_errorIntegral = 0;
static float m_oldValueError = 0;

void MotorSelPositionRegulator_Init(float Kp, float Ki, float Kd,
									float maxIntegral, float maxInValue, float maxOutValue,
									uint32_t diffMsTime)
{
	m_Kp = Kp;
	m_Ki = Ki;
	m_Kd = Kd;
	m_maxIntegral = maxIntegral;
	m_maxInValue = maxInValue;
	m_maxOutValue = maxOutValue;
	m_diffMsTime = diffMsTime;
}

void MotorSelPositionRegulator_SetKp(float Kp)
{
	m_Kp = Kp;
}

void MotorSelPositionRegulator_SetKi(float Ki)
{
	m_Ki = Ki;
}
;

void MotorSelPositionRegulator_SetKd(float Kd)
{
	m_Kd = Kd;
}

void MotorSelPositionRegulator_SetMaxIntegral(float maxIntegral)
{
	m_maxIntegral = maxIntegral;
}

void MotorSelPositionRegulator_SetMaxInValue(float maxInValue)
{
	m_maxInValue = maxInValue;
}

void MotorSelPositionRegulator_SetMaxOutValue(float maxOutValue)
{
	m_maxOutValue = maxOutValue;
}

static inline float Saturation(float value, float maxValue)
{
	if (value > maxValue)
		value = maxValue;
	else if (value < -maxValue)
		value = -maxValue;
	return value;
}

float MotorSelPositionRegulator_Calculate(float targetValue, float actualValue)
{
	targetValue = Saturation(targetValue, m_maxInValue);
	float errorValue =MinRadiusDiastance(actualValue, targetValue);

	m_errorIntegral += errorValue * (m_diffMsTime * 0.001f);
	m_errorIntegral = Saturation(m_errorIntegral, m_maxIntegral);

	float errorDiffValue = (m_oldValueError - errorValue) / (m_diffMsTime * 0.001f);
	m_oldValueError = errorValue;

	float out = (m_Kp * errorValue + m_Ki * m_errorIntegral	+ m_Kd * errorDiffValue);
	out = Saturation(out, m_maxOutValue);

	if (fabs(out) >= m_maxOutValue)
		m_errorIntegral -= errorValue * (m_diffMsTime * 0.001f);

	return out;
}
