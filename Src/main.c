#include <stdint.h>
#include <stdbool.h>
#include "Clock.h"
#include "IrDetectorSel.h"
#include "IrDetectorCol.h"
#include "SysTick.h"
#include "TaskMenager.h"
#include "ColorDetector.h"
#include "MotorDriv/MotorDriv.h"
#include "MotorSel/MotorSel.h"
#include "DebugMonitor.h"
#include "ServiceUart.h"
#include <string.h>


#define MAX_COLOR_LINE 255


static uint8_t m_backColorLine = 0;
static uint8_t m_beginColorLine = 0;
static ColorDetector_Color m_colorLine[MAX_COLOR_LINE] = {0};

static void SlotIrDetectorColRisingEdgeState()
{

	ColorDetector_GetColor();
}

static void SetPositionOfColor()
{
	ColorDetector_Color color = m_colorLine[m_beginColorLine];
	if(color.Blue < color.Green && color.Blue < color.Red)
		MotorSel_SetPosition(1.05);
	else if(color.Blue > color.Green && color.Blue > color.Red)
		MotorSel_SetPosition(1.05*2);
	else if(color.Green > color.Blue && color.Green > color.Red)
		MotorSel_SetPosition(1.05*3);
	else if(color.Red > color.Blue && color.Red > color.Green)
		MotorSel_SetPosition(1.05*4);

}
static void SlotColorReady(ColorDetector_Color color)
{
	m_colorLine[m_backColorLine] = color;
	m_backColorLine++;
}

static void SlotIrDetectorSelRisingEdgeState()
{
	SetPositionOfColor();
	m_beginColorLine++;
}

static inline void Init()
{
	Clock_Init();
	SysTick_Init();
	TaskMenager_Init();
	ServiceUart_Init();
	IrDetectorSel_Init(10);
	IrDetectorCol_Init(10);
	ColorDetector_Init(100);
	MotorDriv_Init(1);
	MotorSel_Init(1);

	MotorDriv_SetSpeed(0.5);

	IrDetectorCol_SetRisingEdgeStateFunc(SlotIrDetectorColRisingEdgeState);
	IrDetectorSel_SetRisingEdgeStateFunc(SlotIrDetectorSelRisingEdgeState);
	ColorDetector_SetColorReadyFunc(SlotColorReady);

}


int main(void)
{
	Init();

	  volatile uint32_t *demcr = (uint32_t*)0xE000EDFC;
	  const uint32_t mon_en_bit = 16;
	  *demcr |= 7 << mon_en_bit;

	  volatile uint32_t *shpr3 = (uint32_t *)0xE000ED20;
	  *shpr3 = 0x00;

	while (true)
	{
		sendData.indexMain++;
		TaskMenager_Run();
	}

}

