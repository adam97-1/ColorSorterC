#include "MotorSel/MotorSel.h"
#include <stm32f446xx.h>
#include <math.h>
#include "TaskMenager.h"
#include "MotorSel/EncoderSel.h"
#include "MotorSel/MotorSelSpeedRegulator.h"
#include "MotorSel/MotorSelPositionRegulator.h"

//IN2 -- PB6/TIM4_CH1
//IN1 -- PA7/TIM14_CH1

static float m_maxPwm = 2000;
static float m_maxSpeed = 15.6;

static float m_position = 0;

static uint32_t m_msPeriod = 1;

static void Loop()
{

	float speedOut = MotorSelPositionRegulator_Calculate(m_position, EncoderSel_GetPosition());
	float out = MotorSelSpeedRegulator_Calculate(speedOut, EncoderSel_GetSpeed());

	if (out > 0)
	{
		TIM4->CCR1 = 0;
		TIM14->CCR1 = (uint32_t) out;
	} else
	{
		TIM4->CCR1 = (uint32_t) -out;
		TIM14->CCR1 = 0;
	}
}

void MotorSel_Init(uint32_t msPeriod )
{
	m_msPeriod  = msPeriod;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN | RCC_APB1ENR_TIM14EN;

	GPIOA->MODER &= ~(GPIO_MODER_MODE7_Msk);
	GPIOA->MODER |= (2 << GPIO_MODER_MODE7_Pos);

	GPIOB->MODER &= ~(GPIO_MODER_MODE6_Msk);
	GPIOB->MODER |= (2 << GPIO_MODER_MODE6_Pos);

	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD7_Msk);
	GPIOA->PUPDR |= (2 << GPIO_PUPDR_PUPD7_Pos);

	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD6_Msk);
	GPIOB->PUPDR |= (2 << GPIO_PUPDR_PUPD6_Pos);

	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL7_Msk);
	GPIOA->AFR[0] |= (0b1001 << GPIO_AFRL_AFSEL7_Pos);

	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL6_Msk);
	GPIOB->AFR[0] |= (2 << GPIO_AFRL_AFSEL6_Pos);

	TIM4->CCMR1 &= ~(TIM_CCMR1_OC1M_Msk);
	TIM4->CCMR1 |= (0b111 << TIM_CCMR1_OC1M_Pos | TIM_CCMR1_OC1PE);

	TIM14->CCMR1 &= ~(TIM_CCMR1_OC1M_Msk);
	TIM14->CCMR1 |= (0b111 << TIM_CCMR1_OC1M_Pos | TIM_CCMR1_OC1PE);

	TIM4->CCER |= (TIM_CCER_CC1E);

	TIM14->CCER |= (TIM_CCER_CC1E);

	TIM4->ARR = m_maxPwm;
	TIM14->ARR = m_maxPwm;

	TIM4->CR1 |= (TIM_CR1_CEN);
	TIM14->CR1 |= (TIM_CR1_CEN);

	MotorSelSpeedRegulator_Init(300, 1000, 0, 1000000, m_maxSpeed, m_maxPwm, m_msPeriod);
	MotorSelPositionRegulator_Init(20, 20, 0, 1000000, 2*M_PI, m_maxSpeed,	m_msPeriod);

	EncoderSel_Init(m_msPeriod, 2472);

	Task task = { .Func = Loop, .Period = m_msPeriod, .Prioryty = 1 };
	TaskMenager_AddTask(task);
}

void MotorSel_SetPosition(float position)
{
	m_position = position;
}

float MotorSel_GetSpeed()
{
	return EncoderSel_GetSpeed();
}

float MotorSel_GetPosition()
{
	return EncoderSel_GetPosition();
}
