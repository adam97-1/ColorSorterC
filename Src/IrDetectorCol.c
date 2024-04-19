#include "IrDetectorCol.h"
#include <stm32f446xx.h>
#include <stddef.h>
#include "TaskMenager.h"

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
  bool State = IrDetectorCol_GetState();
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

void IrDetectorCol_Init()
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

  GPIOC->MODER &= ~GPIO_MODER_MODE9;
  GPIOC->MODER |= 0 << GPIO_MODER_MODE9_Pos;

  GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD9_Msk;
  GPIOC->PUPDR |= 1 << GPIO_PUPDR_PUPD9_Pos;

  Task task = { .Func = Loop, .Period = 100, .Prioryty = 1};
  TaskMenager_AddTask(task);
}

void IrDetectorCol_SetRisingEdgeStateFunc(void (*func)(void))
{
  RisingEdgeStateFunc = func;
}

void IrDetectorCol_SetFallingEdgeStateFunc(void (*func)(void))
{
  FallingEdgeStateFunc = func;
}

bool IrDetectorCol_GetState()
{
  return (GPIOC->IDR & GPIO_IDR_ID9_Msk) != GPIO_IDR_ID9;
}


