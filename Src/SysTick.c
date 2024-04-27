#include "SysTick.h"
#include <stm32f446xx.h>
#include <stddef.h>

static volatile uint32_t MsTime = 0;

void SysTick_Init()
{
  SysTick_Config(SystemCoreClock/1000);
  NVIC_EnableIRQ(SysTick_IRQn);
 
}

uint32_t SysTick_GetTime()
{
  return MsTime;
}

void SysTick_Handler(void)
{
  MsTime++;
}

