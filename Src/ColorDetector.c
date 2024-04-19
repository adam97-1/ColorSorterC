#include "ColorDetector.h"
#include <stm32f446xx.h>
#include "TaskMenager.h"

static void GpioInit()
{
  RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN);
  RCC->APB1ENR |= (RCC_APB1ENR_TIM3EN);
  
  GPIOC->MODER &= ~(GPIO_MODER_MODE5_Msk      | GPIO_MODER_MODE6_Msk      | GPIO_MODER_MODE10_Msk      |
		    GPIO_MODER_MODE11_Msk     | GPIO_MODER_MODE12_Msk);
  GPIOC->MODER |= (1 << GPIO_MODER_MODE5_Pos  | 1 << GPIO_MODER_MODE6_Pos | 1 << GPIO_MODER_MODE10_Pos |
		   1 << GPIO_MODER_MODE11_Pos | 1 << GPIO_MODER_MODE12_Pos);
  GPIOD->MODER &= ~(GPIO_MODER_MODE2_Msk);
  GPIOD->MODER |= (2 << GPIO_MODER_MODE2_Pos);

  GPIOC->OTYPER &= ~(GPIO_OTYPER_OT5 | GPIO_OTYPER_OT6 | GPIO_OTYPER_OT10 | GPIO_OTYPER_OT11 |
		     GPIO_OTYPER_OT12);

  GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED5_Msk      | GPIO_OSPEEDR_OSPEED6_Msk       |
		      GPIO_OSPEEDR_OSPEED10_Msk     | GPIO_OSPEEDR_OSPEED11_Msk      |
		      GPIO_OSPEEDR_OSPEED12_Msk);
  GPIOC->OSPEEDR |= (0 << GPIO_OSPEEDR_OSPEED5_Pos  | 0 << GPIO_OSPEEDR_OSPEED6_Pos  |
		     0 << GPIO_OSPEEDR_OSPEED10_Pos | 0 << GPIO_OSPEEDR_OSPEED11_Pos |
		     0 << GPIO_OSPEEDR_OSPEED12_Pos);

  GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD5_Msk      | GPIO_PUPDR_PUPD6_Msk      | GPIO_PUPDR_PUPD10_Msk      |
		    GPIO_PUPDR_PUPD11_Msk     | GPIO_PUPDR_PUPD12_Msk);
  GPIOC->PUPDR |= (1 << GPIO_PUPDR_PUPD5_Pos  | 1 << GPIO_PUPDR_PUPD6_Pos | 1 << GPIO_PUPDR_PUPD10_Pos |
		   1 << GPIO_PUPDR_PUPD11_Pos | 1 << GPIO_PUPDR_PUPD12_Pos);

  GPIOD->AFR[0] &= ~(GPIO_AFRL_AFSEL2_Msk);
  GPIOD->AFR[0] |= (2 << GPIO_AFRL_AFSEL2_Pos);

}

static void TimerInit()
{
  TIM3->SMCR &= ~(TIM_SMCR_ETF_Msk     | TIM_SMCR_ETPS_Msk      | TIM_SMCR_ETP);
  TIM3->SMCR |= (0 << TIM_SMCR_ETF_Pos | 1 << TIM_SMCR_ETPS_Pos | TIM_SMCR_ECE);

  TIM3->CR1 |= (TIM_CR1_CEN);
}

static void Loop()
{
  uint32_t Frequency = TIM3->CNT;
  TIM3->CNT = 0;
  GPIOA->ODR ^= GPIO_ODR_OD5;
}

void ColorDetector_Init()
{
  GpioInit();
  TimerInit();
  Task task = {.Func = Loop, .Period = 1000, .Prioryty = 1};
  TaskMenager_AddTask(task);
}
