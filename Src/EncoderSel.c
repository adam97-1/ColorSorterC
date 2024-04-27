#include "EncoderSel.h"
#include <stm32f446xx.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include "TaskMenager.h"

//PA5 -- CC1
//PB3 -- CC2

static float speed = 0;

static void Loop()
{
  static uint16_t  oldPosition = 0;
  uint16_t position = TIM2->CNT;
  if(abs(oldPosition-position) < 1000)
	  speed = (oldPosition-position)*2*M_PI*1000/2500;
  oldPosition = position;

}

void EncoderSel_Init()
{
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
  
  TIM2->CCMR1 &= ~(TIM_CCMR1_CC1S_Msk      | TIM_CCMR1_CC2S_Msk      |
                  TIM_CCMR1_IC1F_Msk      | TIM_CCMR1_IC2F_Msk);
  TIM2->CCMR1 |= (1 << TIM_CCMR1_CC1S_Pos | 1 << TIM_CCMR1_CC2S_Pos  |
                  0 << TIM_CCMR1_IC1F_Pos | 0 << TIM_CCMR1_IC2F_Pos);
  
  TIM2->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P | TIM_CCER_CC1NP | TIM_CCER_CC2NP);

  TIM2->SMCR &= ~(TIM_SMCR_SMS_Msk);
  TIM2->SMCR |= (3 << TIM_SMCR_SMS_Pos);

  TIM2->ARR = 2500;

  TIM2->CR1 |= (TIM_CR1_CEN);

  Task task = { .Func = Loop, .Period = 1, .Prioryty = 1};
  TaskMenager_AddTask(task);
  
}

float EncoderSel_GetSpeed()
{
  return speed;
}

float EncoderSel_GetPosition()
{
  return TIM2->CNT*2*M_PI/2500;
}
