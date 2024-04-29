#include "Clock.h"
#include "IrDetectorSel.h"
#include "IrDetectorCol.h"
#include "SysTick.h"
#include "TaskMenager.h"
#include "ColorDetector.h"
#include "MotorDriv/MotorDriv.h"
#include "MotorSel/MotorSel.h"
//#include "ServiceUart.h"

void SlotColorReady(ColorDetector_Color color)
{

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
	MotorDriv_SetSpeed(10);

	//ServiceUart_Init();

	ColorDetector_SetColorReadyFunc(SlotColorReady);

	while (1)
	{
		TaskMenager_Run();
		SpeedDriv = MotorDriv_GetSpeed();
		SpeedSel = MotorSel_GetSpeed();
		PosSel = MotorSel_GetPosition();
	}

}
