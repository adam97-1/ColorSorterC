#include "ServiceUart.h"
#include <stm32f446xx.h>
#include <stddef.h>
#include "TaskMenager.h"
#include "SysTick.h"
#include "MotorDriv/EncoderDriv.h"
#include "MotorSel/EncoderSel.h"
#include "Crc32.h"

typedef struct
{
	uint32_t header;
	uint32_t msTime;
	float positionMotorSel;
	float speedMotorSel;
	float positionMotorDriv;
	float speedMotorDriv;
	uint32_t crc;
} SeviceUartData;

static uint64_t m_msPeriod = 1;
static SeviceUartData m_sendData =	{ 	.header = 0xAAAAAAAA, .msTime = 1, .positionMotorSel = 1,
										.speedMotorSel = 1, .speedMotorDriv = 1, .crc = 0 };

static void SendDataUart(const void *begin, size_t size)
{
	for (size_t i = 0; i < size; i++) {
		USART2->DR = *(const uint8_t*) (begin + i);
		while (!(USART2->SR & USART_SR_TC))
			;
	}

}

static void Loop()
{
	m_sendData.msTime = SysTick_GetTime();
	m_sendData.positionMotorDriv = EncoderDriv_GetPosition();
	m_sendData.speedMotorDriv = EncoderDriv_GetSpeed();
	m_sendData.positionMotorSel = EncoderSel_GetPosition();
	m_sendData.speedMotorSel = EncoderSel_GetSpeed();

	m_sendData.crc = Crc32_CalcCrc((uint8_t*) (&m_sendData), (size_t) (sizeof(m_sendData) - sizeof(m_sendData.crc)), 0);
	SendDataUart(&m_sendData, (size_t) (sizeof(m_sendData)));
}

void ServiceUart_Init(uint32_t msPeriod )
{
	m_msPeriod  = msPeriod;

	RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN);

	GPIOA->MODER &= ~(GPIO_MODER_MODE2_Msk 		| GPIO_MODER_MODE3_Msk);
	GPIOA->MODER |= (2 << GPIO_MODER_MODE2_Pos 	| 2 << GPIO_MODER_MODE3_Pos);

	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD2_Msk 		| GPIO_PUPDR_PUPD3_Msk);
	GPIOA->PUPDR |= (2 << GPIO_PUPDR_PUPD2_Pos 	| 2 << GPIO_PUPDR_PUPD3_Pos);

	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL2_Msk 			| GPIO_AFRL_AFSEL3_Msk);
	GPIOA->AFR[0] |= (0b0111 << GPIO_AFRL_AFSEL2_Pos	| 0b0111 << GPIO_AFRL_AFSEL3_Pos);

	RCC->APB1ENR |= (RCC_APB1ENR_USART2EN);

	USART2->CR1 |= (USART_CR1_UE);
	USART2->CR1 &= ~(USART_CR1_M);

	USART2->CR2 &= ~(USART_CR2_STOP_Msk);
	USART2->CR2 |= (0 << USART_CR2_STOP_Pos);

	USART2->BRR |= 45000000 / 115200;

	USART2->CR1 |= (USART_CR1_TE);

	Task task = { .Func = Loop, .Period = m_msPeriod, .Prioryty = 1 };
	TaskMenager_AddTask(task);
}
