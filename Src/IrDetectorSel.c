
#include "IrDetectorSel.h"
#include <stm32f446xx.h>

void IrDetectorSel_Init()
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

  GPIOC->MODER &= ~GPIO_MODER_MODE8;
  GPIOC->MODER |= 0 << GPIO_MODER_MODE8_Pos;

  GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD8_Msk;
  GPIOC->PUPDR |= 1 << GPIO_PUPDR_PUPD8_Pos;
}

bool IrDetectorSel_getState()
{
  return (GPIOC->IDR & GPIO_IDR_ID8_Msk) != GPIO_IDR_ID8;
}
