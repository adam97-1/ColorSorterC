#include "MotorDriv/MotorDrivSpeedRegulator.h"

#include "math.h"

static float m_Kp = 1;
static float m_Ki = 0;
static float m_Kd = 0;
static float m_maxIntegral = 10000000;
static float m_maxSpeed = 1000000;
static float m_maxOut = 1000000;
static uint32_t m_diffMsTime = 1;

static float m_speedErrorIntegral = 0;
static float m_oldSpeedError = 0;

void MotorDrivSpeedRegulator_Init(float Kp, float Ki, float Kd, float maxIntegral, float maxSpeed, float maxOut, uint32_t diffMsTime)
{
  m_Kp = Kp;
  m_Ki = Ki;
  m_Kd = Kd;
  m_maxIntegral = maxIntegral;
  m_maxSpeed = maxSpeed;
  m_maxOut = maxOut;
  m_diffMsTime = diffMsTime;
}

void MotorDrivSpeedRegulator_SetKp(float Kp)
{
  m_Kp = Kp;
}

void MotorDrivSpeedRegulator_SetKi(float Ki)
{
  m_Ki = Ki;
};

void MotorDrivSpeedRegulator_SetKd(float Kd)
{
  m_Kd = Kd;
}

void MotorDrivSpeedRegulator_SetMaxIntegral(float maxIntegral)
{
	m_maxIntegral = maxIntegral;
}

void MotorDrivSpeedRegulator_SetMaxSpeed(float maxSpeed)
{
	m_maxSpeed = maxSpeed;
}

void MotorDrivSpeedRegulator_SetMaxOut(float maxOut)
{
	m_maxOut = maxOut;
}

static inline float Saturation(float value, float maxValue)
{
	  if(value > maxValue)
		  value = maxValue;
	  else if(value < -maxValue)
		  value = -maxValue;
	  return value;
}

float MotorDrivSpeedRegulator_SpeedCalculate(float targetSpeed, float actualSpeed)
{
  targetSpeed = Saturation(targetSpeed, m_maxSpeed);
  float speedError = targetSpeed - actualSpeed;
  
  m_speedErrorIntegral += speedError*(m_diffMsTime*0.001f);
  m_speedErrorIntegral = Saturation(m_speedErrorIntegral, m_maxIntegral);

  float speedErrorDiff = (m_oldSpeedError - speedError)*(m_diffMsTime*0.001f);
  m_oldSpeedError = speedError;

  float out = (m_Kp * speedError + m_Ki * m_speedErrorIntegral + m_Kd * speedErrorDiff);
  out = Saturation(out, m_maxOut);

  if(fabs(out) >= m_maxOut)
	  m_speedErrorIntegral -= speedError*(m_diffMsTime*0.001f);

  return out;
}
