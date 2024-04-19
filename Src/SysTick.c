#include "SysTick.h"
#include <stm32f446xx.h>
#include <stddef.h>

void SysTick_Init()
{
  SysTick_Config(SystemCoreClock/1000);
  NVIC_EnableIRQ(SysTick_IRQn);
 
}

void SysTick_Handler(void)
{
  static uint32_t msTime = 0;
  msTime++;

  if(msTime % 100)
    for (size_t i = 9; i < 10; i--)
      {
	if(PeriodFunc[i] == NULL)
	  continue;
	PeriodFunc[i]();
      }
}

void (*PeriodFunc[10])(void) = {NULL};

