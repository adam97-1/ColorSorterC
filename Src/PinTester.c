#include  "PinTester.h"
#include <stm32f446xx.h>

void PinTeste_Init()
{
	RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOCEN);

	GPIOC->MODER &= ~(GPIO_MODER_MODE3_Msk );
	GPIOC->MODER |= (1 << GPIO_MODER_MODE3_Pos);


	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT3);

	GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED3_Msk);
	GPIOC->OSPEEDR |= 	(3 << GPIO_OSPEEDR_OSPEED3_Pos);

	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD5_Msk );
	GPIOC->PUPDR |= (1 << GPIO_PUPDR_PUPD3_Pos);
}

void PinTeste_SetHigh()
{
	GPIOC->ODR |= GPIO_ODR_OD3;
}

void PinTeste_SetLow()
{
	GPIOC->ODR &= ~GPIO_ODR_OD3;
}
