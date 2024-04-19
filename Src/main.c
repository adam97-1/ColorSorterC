#include <stdint.h>
#include <stm32f446xx.h>
#include <system_stm32f4xx.h>
#include "Clock.h"
#include "IrDetectorSel.h"
#include "IrDetectorCol.h"
#include "SysTick.h"

void testFun()
{
  GPIOA->ODR ^= GPIO_ODR_OD5;
}

int main(void)
{
  Clock_Init();
  SystemCoreClockUpdate();
  IrDetectorSel_Init();
  IrDetectorCol_Init();



  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

  GPIOA->MODER |= GPIO_MODER_MODE5_0;
  GPIOA->MODER &= ~(GPIO_MODER_MODE5_1);

  GPIOA->OTYPER &= ~(GPIO_OTYPER_OT5);

  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5_0;
  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5_1;

  GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_0;
  GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD0_1);
  IrDetectorCol_SetFallingEngeStateFunc(testFun);
  IrDetectorCol_SetRisingEngeStateFunc(testFun);
  SysTick_Init();
  while(1)
    {
      
	   
    }
}
