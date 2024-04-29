#include <stdint.h>
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
	MotorDriv_SetSpeed(0);
	ColorDetector_GetColor();
}

static void SetPositionOfColor()
{
	MotorSel_SetPosition(1);
}

static void SlotColorReady(ColorDetector_Color color)
{
	m_colorLine[m_backColorLine] = color;
	m_backColorLine++;
	MotorDriv_SetSpeed(0.5);
	SetPositionOfColor();
}

static void SlotIrDetectorSelRisingEdgeState()
{
	SetPositionOfColor();
	m_beginColorLine++;
}

float SpeedDriv;
float PosSel;
float SpeedSel;
int main(void)
{
	Clock_Init();
	SysTick_Init();
	IrDetectorSel_Init(100);
	IrDetectorCol_Init(100);
	ColorDetector_Init(300);
	MotorDriv_Init(1);
	MotorSel_Init(1);
	//ServiceUart_Init();

	IrDetectorCol_SetRisingEdgeStateFunc(SlotIrDetectorColRisingEdgeState);
	IrDetectorSel_SetRisingEdgeStateFunc(SlotIrDetectorSelRisingEdgeState);
	ColorDetector_SetColorReadyFunc(SlotColorReady);


	while (1)
	{
		TaskMenager_Run();
		SpeedDriv = MotorDriv_GetSpeed();
		SpeedSel = MotorSel_GetSpeed();
		PosSel = MotorSel_GetPosition();
	}

}
