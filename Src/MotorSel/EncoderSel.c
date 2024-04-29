#include "MotorSel/EncoderSel.h"
#include <stm32f446xx.h>
#include <math.h>
#include "TaskMenager.h"
#include "Functions.h"

//PA5 -- CC1
//PB3 -- CC2

static float m_speed = 0;
static uint32_t m_msPeriod = 1;
static uint32_t m_maxEncoderValue = 2500;

static void Loop()
{
	static float oldPosition = 0;
	float position = EncoderSel_GetPosition();

	float diffPosition = MinRadiusDiastance(oldPosition, position);

	m_speed = diffPosition / (m_msPeriod * 0.001f);

	oldPosition = position;
}

void EncoderSel_Init(uint32_t msPeriod, uint32_t maxEncoderValue)
{
	m_msPeriod = msPeriod;
	m_maxEncoderValue = maxEncoderValue;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	GPIOA->MODER &= ~(GPIO_MODER_MODE5_Msk);
	GPIOA->MODER |= (2 << GPIO_MODER_MODE5_Pos);

	GPIOB->MODER &= ~(GPIO_MODER_MODE3_Msk);
	GPIOB->MODER |= (2 << GPIO_MODER_MODE3_Pos);

	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD5_Msk);
	GPIOA->PUPDR |= (2 << GPIO_PUPDR_PUPD5_Pos);

	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD3_Msk);
	GPIOB->PUPDR |= (2 << GPIO_PUPDR_PUPD3_Pos);

	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL5_Msk);
	GPIOA->AFR[0] |= (1 << GPIO_AFRL_AFSEL5_Pos);

	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL3_Msk);
	GPIOB->AFR[0] |= (1 << GPIO_AFRL_AFSEL3_Pos);

	TIM2->CCMR1 &= ~(TIM_CCMR1_CC1S_Msk | TIM_CCMR1_CC2S_Msk |
					TIM_CCMR1_IC1F_Msk 	| TIM_CCMR1_IC2F_Msk);
	TIM2->CCMR1 |= (1 << TIM_CCMR1_CC1S_Pos | 1 << TIM_CCMR1_CC2S_Pos |
					0 << TIM_CCMR1_IC1F_Pos | 0 << TIM_CCMR1_IC2F_Pos);

	TIM2->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P |
					TIM_CCER_CC1NP| TIM_CCER_CC2NP);

	TIM2->SMCR &= ~(TIM_SMCR_SMS_Msk);
	TIM2->SMCR |= (3 << TIM_SMCR_SMS_Pos);

	TIM2->ARR = m_maxEncoderValue;

	TIM2->CR1 |= (TIM_CR1_CEN);

	Task task = { .Func = Loop, .Period = m_msPeriod, .Prioryty = 1 };
	TaskMenager_AddTask(task);

}

void EncoderSel_SetMaxEncoderValue(uint32_t maxEncoderValue)
{
	m_maxEncoderValue = maxEncoderValue;
}

float EncoderSel_GetSpeed()
{
	return m_speed;
}

float EncoderSel_GetPosition()
{
	return TIM1->CNT * 2 * M_PI / m_maxEncoderValue;
}
