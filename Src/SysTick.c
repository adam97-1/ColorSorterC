#include "SysTick.h"
#include <stm32f446xx.h>
#include <stddef.h>

static uint32_t msTime = 0;

void SysTick_Init()
{
  SysTick_Config(SystemCoreClock/1000);
  NVIC_EnableIRQ(SysTick_IRQn);
 
}

uint32_t SysTick_GetTime()
{
  return msTime;
}

void SysTick_Handler(void)
{
  msTime++;
}

