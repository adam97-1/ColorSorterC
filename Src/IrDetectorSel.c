#include "IrDetectorSel.h"
#include <stm32f446xx.h>
#include <stddef.h>
#include "SysTick.h"

static void (*RisingEdgeStateFunc)(void) = NULL;
static void (*FallingEdgeStateFunc)(void) = NULL;

static void RisingEdgeState()
{
  if(RisingEdgeStateFunc == NULL)
    return;
  RisingEdgeStateFunc();
}

static void FallingEdgeState()
{
  if(FallingEdgeStateFunc == NULL)
    return;
  FallingEdgeStateFunc();
}

static void Loop()
{ 
  static bool OldState = false;
  bool State = IrDetectorSel_GetState();
  if(State != OldState)
    {
      if(State == true)
	RisingEdgeState();
      else
	FallingEdgeState();
      OldState = State;
    }
  else;
}

void IrDetectorSel_Init()
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

  GPIOC->MODER &= ~GPIO_MODER_MODE8;
  GPIOC->MODER |= 0 << GPIO_MODER_MODE8_Pos;

  GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD8_Msk;
  GPIOC->PUPDR |= 1 << GPIO_PUPDR_PUPD8_Pos;

  PeriodFunc[1] = Loop;
}

void IrDetectorSel_SetRisingEdgeStateFunc(void (*func)(void))
{
  RisingEdgeStateFunc = func;
}

void IrDetectorSel_SetFallingEdgeStateFunc(void (*func)(void))
{
  FallingEdgeStateFunc = func;
}

bool IrDetectorSel_GetState()
{
  return (GPIOC->IDR & GPIO_IDR_ID8_Msk) != GPIO_IDR_ID8;
}
