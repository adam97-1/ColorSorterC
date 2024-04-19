#include <stdint.h>
#include <stm32f446xx.h>
#include <system_stm32f4xx.h>
#include "Clock.h"
#include "IrDetectorSel.h"
#include "IrDetectorCol.h"
#include "SysTick.h"

void ToggleLed2()
{
  GPIOA->ODR ^= GPIO_ODR_OD5;
}

void InitLed2()
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

  GPIOA->MODER |= GPIO_MODER_MODE5_0;
  GPIOA->MODER &= ~(GPIO_MODER_MODE5_1);

  GPIOA->OTYPER &= ~(GPIO_OTYPER_OT5);

  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5_0;
  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5_1;

  GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_0;
  GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD0_1);
}

int main(void)
{
  Clock_Init();
  SystemCoreClockUpdate();
  SysTick_Init();
  IrDetectorSel_Init();
  IrDetectorCol_Init();

  InitLed2();
  
  IrDetectorCol_SetFallingEdgeStateFunc(ToggleLed2);
  IrDetectorCol_SetRisingEdgeStateFunc(ToggleLed2);
  
  IrDetectorSel_SetFallingEdgeStateFunc(ToggleLed2);
  IrDetectorSel_SetRisingEdgeStateFunc(ToggleLed2);
  
  while(1)
    {
      
	   
    }
}
