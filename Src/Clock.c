#include "Clock.h"
#include <stm32f446xx.h>
#include <system_stm32f4xx.h>
#include "ClearStack.h"

void Clock_Init() {
	ADD_TO_CLEAR();
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	PWR->CR |= (3 << PWR_CR_VOS_Pos);

	FLASH->ACR |= 	(FLASH_ACR_PRFTEN 	| FLASH_ACR_ICEN 		|
					FLASH_ACR_DCEN		| FLASH_ACR_LATENCY_5WS);

	RCC->CFGR &= ~(RCC_CFGR_PPRE1_Msk | RCC_CFGR_PPRE2_Msk  | RCC_CFGR_HPRE_Msk);
	RCC->CFGR |= (RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2 | RCC_CFGR_HPRE_DIV1);

	RCC->CFGR &= ~(RCC_CFGR_PPRE1_Msk | RCC_CFGR_PPRE2_Msk  | RCC_CFGR_HPRE_Msk);
	RCC->CFGR |= (RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2 | RCC_CFGR_HPRE_DIV1);

	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM_Msk 			| RCC_PLLCFGR_PLLN_Msk 			|
					RCC_PLLCFGR_PLLP_Msk);
	RCC->PLLCFGR |= (8 << RCC_PLLCFGR_PLLM_Pos	| 180 << RCC_PLLCFGR_PLLN_Pos |
					0 << RCC_PLLCFGR_PLLP_Pos 	| RCC_PLLCFGR_PLLSRC_HSI);

	RCC->CR |= RCC_CR_PLLON;
	while ((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY)
		;

	RCC->CFGR &= ~RCC_CFGR_SW_Msk;
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL)
		;

	SystemCoreClockUpdate();
	CLEAR_STACK();

}
