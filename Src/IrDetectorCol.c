#include "IrDetectorCol.h"
#include <stm32f446xx.h>
#include <stddef.h>
#include "SysTick.h"

void (*RisingEdgeState)(void) = NULL;
void (*FallingEdgeState)(void) = NULL;

static void IrDetectorCol_Loop()
{
  static bool oldState = false;
  bool state = IrDetectorCol_GetState();
  if(state != oldState)
    {
    if(state == true)
      RisingEdgeState();
    else
      FallingEdgeState();
    oldState = state;
    }
  else;
}

void IrDetectorCol_Init()
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

  GPIOC->MODER &= ~GPIO_MODER_MODE9;
  GPIOC->MODER |= 0 << GPIO_MODER_MODE9_Pos;

  GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD9_Msk;
  GPIOC->PUPDR |= 1 << GPIO_PUPDR_PUPD9_Pos;

  PeriodFunc[0] = IrDetectorCol_Loop;
}

void IrDetectorCol_SetRisingEngeStateFunc(void (*func)(void))
{
  RisingEdgeState = func;
}

void IrDetectorCol_SetFallingEngeStateFunc(void (*func)(void))
{
  FallingEdgeState = func;
}

bool IrDetectorCol_GetState()
{
  return (GPIOC->IDR & GPIO_IDR_ID9_Msk) != GPIO_IDR_ID9;
}


