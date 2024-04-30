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
ColorDetector_Color m_color;
static void SlotIrDetectorColRisingEdgeState()
{

	ColorDetector_GetColor();
	MotorDriv_SetSpeed(0);
}

static void SetPositionOfColor()
{
	MotorSel_SetPosition(1);
}

static void SlotColorReady(ColorDetector_Color color)
{
	m_colorLine[m_backColorLine] = color;
	m_color = color;
	m_backColorLine++;
//	MotorDriv_SetSpeed(1);
//	SetPositionOfColor();
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
	IrDetectorSel_Init(100);
	IrDetectorCol_Init(100);
	ColorDetector_Init(10);
	MotorDriv_Init(1);
	MotorSel_Init(1);
	//ServiceUart_Init();

//	MotorDriv_SetSpeed(1);

	IrDetectorCol_SetRisingEdgeStateFunc(SlotIrDetectorColRisingEdgeState);
	IrDetectorSel_SetRisingEdgeStateFunc(SlotIrDetectorSelRisingEdgeState);
	ColorDetector_SetColorReadyFunc(SlotColorReady);

}

int main(void)
{
	Init();


	while (true)
	{
		TaskMenager_Run();
		ColorDetector_GetColor();
	}

}
