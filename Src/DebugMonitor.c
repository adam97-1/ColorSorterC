#include "DebugMonitor.h"
#include "Crc32.h"

volatile SeviceUartData sendData = {.header = 0x21212121, .indexMain = 0x01010101, .indexStack = 0x02020202, .stackPointer = 0x03030303, .usedMem = 0x04040404, .crc = 0x05050505};
uint32_t indexMain = 0;

void DebugMon_Handler(void)
{
//	uint8_t *sp;
//	__asm__("mov %0, sp" : "=r"(sp));
//
//	if((uint8_t*)sendData.stackPointer < sp)
//	{
////		for(uint8_t* start = (uint8_t*)sendData.stackPointer; start <= sp; start++)
////			*start = 0xAA;
//		sendData.indexStack++;
//	}
//	else if((uint8_t*)sendData.stackPointer > sp)
//	{
//		sendData.indexStack--;
//	}
//	else
//	{
//		return;
//	}
//
//	sendData.stackPointer = sp;
//	sendData.usedMem = 0;
//	for(uint8_t* start = (uint8_t*)0x20000000; start < sp; start++)
//	{
//		if(*start == 0xAA)
//			sendData.usedMem++;
//	}
//
//	sendData.crc = CRC32_CALC_CRC((uint8_t*) (&sendData), (size_t) (sizeof(sendData) - sizeof(sendData.crc)), 0);
	SEND_DATA_UART(&sendData, (sizeof(SeviceUartData)));
	SEND_DATA_UART("????", 4);
	while(indexMain++ < 1000000);
	indexMain =0;

}
