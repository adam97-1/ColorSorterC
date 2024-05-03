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
#include "ClearStack.h"
#include "Wrap.h"

//#include "ServiceUart.h"

#define MAX_COLOR_LINE 255

static uint8_t m_backColorLine = 0;
static uint8_t m_beginColorLine = 0;
static ColorDetector_Color m_colorLine[MAX_COLOR_LINE] = {0};
ColorDetector_Color m_color;
static void SlotIrDetectorColRisingEdgeState()
{
	ADD_TO_CLEAR();
	ColorDetector_GetColor();
	CLEAR_STACK();
	MotorDriv_SetSpeed(0);
	CLEAR_STACK();
}

static void SetPositionOfColor()
{
	ADD_TO_CLEAR();
	MotorSel_SetPosition(1);
	CLEAR_STACK();
}

static void SlotColorReady(ColorDetector_Color color)
{
	ADD_TO_CLEAR();
	m_colorLine[m_backColorLine] = color;
	m_color = color;
	m_backColorLine++;
//	MotorDriv_SetSpeed(1);
//	CLEAR_STACK();
//	SetPositionOfColor();
//	CLEAR_STACK();
}

static void SlotIrDetectorSelRisingEdgeState()
{
	ADD_TO_CLEAR();
	SetPositionOfColor();
	m_beginColorLine++;
}

static inline void Init()
{
	ADD_TO_CLEAR();
	Clock_Init();
	CLEAR_STACK();
	SysTick_Init();
	CLEAR_STACK();
	TaskMenager_Init();
	CLEAR_STACK();
	IrDetectorSel_Init(100);
	CLEAR_STACK();
	IrDetectorCol_Init(100);
	CLEAR_STACK();
	ColorDetector_Init(10);
	CLEAR_STACK();
	MotorDriv_Init(1);
	CLEAR_STACK();
	MotorSel_Init(1);
	CLEAR_STACK();

	//ServiceUart_Init();

//	MotorDriv_SetSpeed(1);
	IrDetectorCol_SetRisingEdgeStateFunc(SlotIrDetectorColRisingEdgeState);
	CLEAR_STACK();
	IrDetectorSel_SetRisingEdgeStateFunc(SlotIrDetectorSelRisingEdgeState);
	CLEAR_STACK();
	ColorDetector_SetColorReadyFunc(SlotColorReady);
	CLEAR_STACK();


}

int main(void)
{
	uint8_t* test = malloc(100);
	memset(test, 0xFF, 100);
	free(test);
	ADD_TO_CLEAR();
	Init();
	CLEAR_STACK();


	while (true)
	{
		TaskMenager_Run();
		CLEAR_STACK();
		ColorDetector_GetColor();
		CLEAR_STACK();
	}

}
