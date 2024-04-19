#include "SysTick.h"
#include <stm32f446xx.h>
#include <stddef.h>

void SysTick_Init()
{
  SysTick_Config(SystemCoreClock/(8000*1000));
  NVIC_EnableIRQ(SysTick_IRQn);
 
}

void SysTick_Handler(void)
{
  static uint32_t msTime = 0;
  msTime++;

  if(msTime % 100)
    PeriodFunc[0]();
}

void (*PeriodFunc[10])(void) = {NULL};

