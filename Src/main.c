#include <stdint.h>
#include <stm32f446xx.h>
#include <system_stm32f4xx.h>
#include "Clock.h"
#include "IrDetectorSel.h"
#include "IrDetectorCol.h"
#include "SysTick.h"
#include "TaskMenager.h"
#include "ColorDetector.h"
#include "EncoderSel.h"
#include "EncoderDriv.h"
#include <stdint.h>
void ToggleLed2()
{
  GPIOA->ODR ^= GPIO_ODR_OD5;
}

void InitLed2()
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

  GPIOA->MODER |= GPIO_MODER_MODE5_0;
  GPIOA->MODER &= ~(GPIO_MODER_MODE5_1);

  GPIOA->OTYPER &= ~(GPIO_OTYPER_OT5);

  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5_0;
  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5_1;

  GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_0;
  GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD0_1);
}

void InitB1()
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

  GPIOC->MODER &= ~GPIO_MODER_MODE9;
  GPIOC->MODER |= 0 << GPIO_MODER_MODE13_Pos;

  GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13_Msk;
  GPIOC->PUPDR |= 1 << GPIO_PUPDR_PUPD13_Pos;

}

void SlotColorReady(ColorDetector_Color color)
{

}
float pos;
float speed;
int main(void)
{
  Clock_Init();
  SystemCoreClockUpdate();
  SysTick_Init();
  IrDetectorSel_Init();
  IrDetectorCol_Init();
  ColorDetector_Init();
  EncoderSel_Init();
  EncoderDriv_Init();

  InitLed2();
  InitB1();
  
  IrDetectorCol_SetFallingEdgeStateFunc(ToggleLed2);
  IrDetectorCol_SetRisingEdgeStateFunc(ToggleLed2);
  
  IrDetectorSel_SetFallingEdgeStateFunc(ToggleLed2);
  IrDetectorSel_SetRisingEdgeStateFunc(ToggleLed2);
  ColorDetector_SetColorReadyFunc(SlotColorReady);
  
  while(1)
    {
      TaskMenager_Run();
      ColorDetector_GetColor();
      pos = EncoderDriv_GetPosition();
      speed = EncoderDriv_GetSpeed();
    }
}
