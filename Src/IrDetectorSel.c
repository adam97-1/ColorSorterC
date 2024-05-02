#include "IrDetectorSel.h"
#include <stm32f446xx.h>
#include <stddef.h>
#include "TaskMenager.h"
#include "ClearStack.h"

static void (*m_risingEdgeStateFunc)(void) = NULL;
static void (*m_fallingEdgeStateFunc)(void) = NULL;
static uint32_t m_msPeriod = 100;

static void RisingEdgeState()
{
	ADD_TO_CLEAR();
	if (m_risingEdgeStateFunc == NULL)
		return;
	m_risingEdgeStateFunc();
	CLEAR_STACK();
}

static void FallingEdgeState()
{
	ADD_TO_CLEAR();
	if (m_fallingEdgeStateFunc == NULL)
		return;
	m_fallingEdgeStateFunc();
	CLEAR_STACK();
}

static void Loop()
{
	ADD_TO_CLEAR();
	static bool oldState = false;
	bool state = IrDetectorSel_GetState();
	CLEAR_STACK();
	if (state != oldState) {
		if (state == true)
		{
			RisingEdgeState();
			CLEAR_STACK();
		}
		else
		{
			FallingEdgeState();
			CLEAR_STACK();
		}
		oldState = state;
	} else
		;
}

void IrDetectorSel_Init(uint32_t msPeriod )
{
	ADD_TO_CLEAR();
	m_msPeriod  = msPeriod;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	GPIOC->MODER &= ~GPIO_MODER_MODE8;
	GPIOC->MODER |= 0 << GPIO_MODER_MODE8_Pos;

	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD8_Msk;
	GPIOC->PUPDR |= 1 << GPIO_PUPDR_PUPD8_Pos;

	Task task = { .Func = Loop, .Period = m_msPeriod, .Prioryty = 1 };
	TaskMenager_AddTask(task);
	CLEAR_STACK();
}

void IrDetectorSel_SetRisingEdgeStateFunc(void (*func)(void))
{
	ADD_TO_CLEAR();
	m_risingEdgeStateFunc = func;
}

void IrDetectorSel_SetFallingEdgeStateFunc(void (*func)(void))
{
	ADD_TO_CLEAR();
	m_fallingEdgeStateFunc = func;
}

bool IrDetectorSel_GetState()
{
	ADD_TO_CLEAR();
	return (GPIOC->IDR & GPIO_IDR_ID8_Msk) != GPIO_IDR_ID8;
}
