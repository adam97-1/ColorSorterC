#include "MotorDriv/MotorDriv.h"
#include <stm32f446xx.h>
#include <stdint.h>
#include "TaskMenager.h"
#include "MotorDriv/EncoderDriv.h"
#include "MotorDriv/MotorDrivSpeedRegulator.h"

//IN2 -- PB8/TIM4_CH3
//IN1 -- PB9/TIM4_CH4

static float m_maxPwm = 2000;
static float m_maxSpeed = 15.6;

static float m_speed = 0;

static uint32_t m_period = 10;

static void Loop()
{
	float out = MotorDrivSpeedRegulator_SpeedCalculate(m_speed, EncoderDriv_GetSpeed());

	if(out > 0)
	{
		TIM4->CCR3 = 0;
		TIM4->CCR4 = (uint32_t)out;
	}
	else
	{
		TIM4->CCR3 = (uint32_t)-out;;
		TIM4->CCR4 = 0;
	}

}

void MotorDriv_Init()
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

  GPIOB->MODER &= ~(GPIO_MODER_MODE8_Msk     | GPIO_MODER_MODE9_Msk);
  GPIOB->MODER |= (2 << GPIO_MODER_MODE8_Pos | 2 << GPIO_MODER_MODE9_Pos);
  

  GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD8_Msk     | GPIO_PUPDR_PUPD9_Msk);
  GPIOB->PUPDR |= (2 << GPIO_PUPDR_PUPD8_Pos | 2 << GPIO_PUPDR_PUPD9_Pos);

  GPIOB->AFR[1] &= ~(GPIO_AFRH_AFSEL8_Msk     | GPIO_AFRH_AFSEL9_Msk);
  GPIOB->AFR[1] |= (2 << GPIO_AFRH_AFSEL8_Pos | 2 << GPIO_AFRH_AFSEL9_Pos);

  TIM4->CCMR2 &= ~(TIM_CCMR2_OC3M_Msk         | TIM_CCMR2_OC4M_Msk);
  TIM4->CCMR2 |= (0b111 << TIM_CCMR2_OC3M_Pos | 0b111 << TIM_CCMR2_OC4M_Pos |
                  TIM_CCMR2_OC3PE             | TIM_CCMR2_OC4PE);
  
  TIM4->CCER |= (TIM_CCER_CC3E | TIM_CCER_CC4E);

  TIM4->ARR = m_maxPwm;

  TIM4->CR1 |= (TIM_CR1_CEN);

  MotorDrivSpeedRegulator_Init(50, 0, 0, 1000000, m_maxSpeed, m_maxPwm, m_period);
  EncoderDriv_Init(m_period, 2500);

  Task task = {.Func = Loop, .Period = m_period, .Prioryty = 1};
  TaskMenager_AddTask(task);
}
void MotorDriv_SetSpeed(float speed)
{
	m_speed = speed;
}
float MotorDriv_GetSpeed()
{
	return EncoderDriv_GetSpeed();
}
