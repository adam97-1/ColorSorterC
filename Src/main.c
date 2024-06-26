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

//#include "ServiceUart.h"

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
	IrDetectorSel_Init(10);
	IrDetectorCol_Init(10);
	ColorDetector_Init(100);
	MotorDriv_Init(1);
	MotorSel_Init(1);
	//ServiceUart_Init();

	MotorDriv_SetSpeed(0.5);

	IrDetectorCol_SetRisingEdgeStateFunc(SlotIrDetectorColRisingEdgeState);
	IrDetectorSel_SetRisingEdgeStateFunc(SlotIrDetectorSelRisingEdgeState);
	ColorDetector_SetColorReadyFunc(SlotColorReady);

}


int main(void)
{
	Init();

	while (true)
		TaskMenager_Run();

}
