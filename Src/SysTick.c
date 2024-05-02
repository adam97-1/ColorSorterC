#include "SysTick.h"
#include <stm32f446xx.h>
#include <stddef.h>
#include "ClearStack.h"

static volatile uint32_t m_msTime = 0;

void SysTick_Init()
{
	ADD_TO_CLEAR();
	SysTick_Config(SystemCoreClock / 1000);
	NVIC_EnableIRQ(SysTick_IRQn);

}

uint32_t SysTick_GetTime() {
	ADD_TO_CLEAR();
	return m_msTime;
}

void SysTick_Handler(void) {
	m_msTime++;
}

