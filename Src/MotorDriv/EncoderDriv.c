#include "MotorDriv/EncoderDriv.h"
#include <stm32f446xx.h>
#include <math.h>
#include "TaskMenager.h"
#include "Functions.h"

//PA9 -- CC1
//PA8 -- CC2

static float m_speed = 0;
static uint32_t m_msPeriod = 1;
static uint32_t m_maxEncoderValue = 2500;

static void Loop()
{
	static float oldPosition = 0;

	float position = EncoderDriv_GetPosition();

	float diffPosition = MinRadiusDiastance(oldPosition, position);

	m_speed = diffPosition / (m_msPeriod * 0.001f);

	oldPosition = position;
}

void EncoderDriv_Init(uint32_t msPeriod, uint32_t maxEncoderValue)
{
	m_msPeriod = msPeriod;
	m_maxEncoderValue = maxEncoderValue;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	GPIOA->MODER &= ~(GPIO_MODER_MODE8_Msk 		| GPIO_MODER_MODE9_Msk);
	GPIOA->MODER |= (2 << GPIO_MODER_MODE8_Pos 	| 2 << GPIO_MODER_MODE9_Pos);

	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD8_Msk 		| GPIO_PUPDR_PUPD9_Msk);
	GPIOA->PUPDR |= (2 << GPIO_PUPDR_PUPD8_Pos 	| 2 << GPIO_PUPDR_PUPD9_Pos);

	GPIOA->AFR[1] &= ~(GPIO_AFRH_AFSEL8_Msk 	| GPIO_AFRH_AFSEL9_Msk);
	GPIOA->AFR[1] |= (1 << GPIO_AFRH_AFSEL8_Pos | 1 << GPIO_AFRH_AFSEL9_Pos);

	TIM1->CCMR1 &= ~(TIM_CCMR1_CC1S_Msk | TIM_CCMR1_CC2S_Msk |
					TIM_CCMR1_IC1F_Msk 	| TIM_CCMR1_IC2F_Msk);
	TIM1->CCMR1 |= (1 << TIM_CCMR1_CC1S_Pos | 1 << TIM_CCMR1_CC2S_Pos	|
					0 << TIM_CCMR1_IC1F_Pos | 0 << TIM_CCMR1_IC2F_Pos);

	TIM1->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P |
					TIM_CCER_CC1NP| TIM_CCER_CC2NP);

	TIM1->SMCR &= ~(TIM_SMCR_SMS_Msk);
	TIM1->SMCR |= (3 << TIM_SMCR_SMS_Pos);

	TIM1->ARR = m_maxEncoderValue;

	TIM1->CR1 |= (TIM_CR1_CEN);

	Task task = { .Func = Loop, .Period = m_msPeriod, .Prioryty = 1 };
	TaskMenager_AddTask(task);
}

void EncoderDriv_SetMaxEncoderValue(uint32_t maxEncoderValue)
{
	m_maxEncoderValue = maxEncoderValue;
}

float EncoderDriv_GetSpeed()
{
	return m_speed;
}

float EncoderDriv_GetPosition()
{
	return TIM1->CNT * 2 * M_PI / m_maxEncoderValue;
}
