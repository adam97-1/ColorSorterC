#pragma once

#include <stdint.h>
#include "stm32f446xx.h"
#include <stddef.h>

#pragma pack(push, n)
typedef struct
{
	uint32_t header;
	uint32_t indexMain;
	int32_t indexStack;
	uint32_t stackPointer;
	uint32_t usedMem;
	uint32_t crc;
} SeviceUartData;
#pragma pack(pop)

#define SEND_DATA_UART(begin, size) {\
    const volatile uint8_t *ptr = (const volatile uint8_t *)(begin); \
    size_t sz = (size); \
    for (size_t i = 0; i < sz; i++) { \
    	while (!(USART2->SR & USART_SR_TXE)); \
        USART2->DR = *(ptr); \
        ptr++; \
    } \
}

void ServiceUart_Init();

  
