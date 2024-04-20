#include "ColorDetector.h"
#include <stm32f446xx.h>
#include "TaskMenager.h"

// S0  - PC6
// S1  - PC5
// S2  - PC10
// S3  - PC11
// LED - PC12
// OUT - PD2/TIM3_ETR

typedef enum
  {
    ColorFilter_None,
    ColorFilter_Red,
    ColorFilter_Green,
    ColorFilter_Blue
  }ColorFilter; 

typedef enum
  {
    Prescaler_0,
    Prescaler_2,
    Prescaler_20,
    Prescaler_100
  }Prescaler;

typedef enum
  {
    PinState_High,
    PinState_Low
  }PinState;

static inline void SetStateS0(PinState State)
{
  switch(State)
    {
    case PinState_High:
      GPIOC->ODR |= GPIO_ODR_OD6;
      break;
    case PinState_Low:
      GPIOC->ODR &= ~GPIO_ODR_OD6;
      break;
    default:
      break;
    }
}

static inline void SetStateS1(PinState State)
{
  switch(State)
    {
    case PinState_High:
      GPIOC->ODR |= GPIO_ODR_OD5;
      break;
    case PinState_Low:
      GPIOC->ODR &= ~GPIO_ODR_OD5;
      break;
    default:
      break;
    }
}
static inline void SetStateS2(PinState State)
{
  switch(State)
    {
    case PinState_High:
      GPIOC->ODR |= GPIO_ODR_OD10;
      break;
    case PinState_Low:
      GPIOC->ODR &= ~GPIO_ODR_OD10;
      break;
    default:
      break;
    }
}
static inline void SetStateS3(PinState State)
{
  switch(State)
    {
    case PinState_High:
      GPIOC->ODR |= GPIO_ODR_OD11;
      break;
    case PinState_Low:
      GPIOC->ODR &= ~GPIO_ODR_OD11;
      break;
    default:
      break;
    }
}

static inline void SetStateLed(PinState State)
{
  switch(State)
    {
    case PinState_High:
      GPIOC->ODR |= GPIO_ODR_OD12;
      break;
    case PinState_Low:
      GPIOC->ODR &= ~GPIO_ODR_OD12;
      break;
    default:
      break;
    }
}

static inline void GpioInit()
{
  RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN);
  RCC->APB1ENR |= (RCC_APB1ENR_TIM3EN);
  
  GPIOC->MODER &= ~(GPIO_MODER_MODE5_Msk      | GPIO_MODER_MODE6_Msk      | GPIO_MODER_MODE10_Msk      |
		    GPIO_MODER_MODE11_Msk     | GPIO_MODER_MODE12_Msk);
  GPIOC->MODER |= (1 << GPIO_MODER_MODE5_Pos  | 1 << GPIO_MODER_MODE6_Pos | 1 << GPIO_MODER_MODE10_Pos |
		   1 << GPIO_MODER_MODE11_Pos | 1 << GPIO_MODER_MODE12_Pos);
  GPIOD->MODER &= ~(GPIO_MODER_MODE2_Msk);
  GPIOD->MODER |= (2 << GPIO_MODER_MODE2_Pos);

  GPIOC->OTYPER &= ~(GPIO_OTYPER_OT5 | GPIO_OTYPER_OT6 | GPIO_OTYPER_OT10 | GPIO_OTYPER_OT11 |
		     GPIO_OTYPER_OT12);

  GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED5_Msk      | GPIO_OSPEEDR_OSPEED6_Msk       |
		      GPIO_OSPEEDR_OSPEED10_Msk     | GPIO_OSPEEDR_OSPEED11_Msk      |
		      GPIO_OSPEEDR_OSPEED12_Msk);
  GPIOC->OSPEEDR |= (0 << GPIO_OSPEEDR_OSPEED5_Pos  | 0 << GPIO_OSPEEDR_OSPEED6_Pos  |
		     0 << GPIO_OSPEEDR_OSPEED10_Pos | 0 << GPIO_OSPEEDR_OSPEED11_Pos |
		     0 << GPIO_OSPEEDR_OSPEED12_Pos);

  GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD5_Msk      | GPIO_PUPDR_PUPD6_Msk      | GPIO_PUPDR_PUPD10_Msk      |
		    GPIO_PUPDR_PUPD11_Msk     | GPIO_PUPDR_PUPD12_Msk);
  GPIOC->PUPDR |= (1 << GPIO_PUPDR_PUPD5_Pos  | 1 << GPIO_PUPDR_PUPD6_Pos | 1 << GPIO_PUPDR_PUPD10_Pos |
		   1 << GPIO_PUPDR_PUPD11_Pos | 1 << GPIO_PUPDR_PUPD12_Pos);

  GPIOD->AFR[0] &= ~(GPIO_AFRL_AFSEL2_Msk);
  GPIOD->AFR[0] |= (2 << GPIO_AFRL_AFSEL2_Pos);

}

static inline void TimerInit()
{
  TIM3->SMCR &= ~(TIM_SMCR_ETF_Msk     | TIM_SMCR_ETPS_Msk      | TIM_SMCR_ETP);
  TIM3->SMCR |= (0 << TIM_SMCR_ETF_Pos | 1 << TIM_SMCR_ETPS_Pos | TIM_SMCR_ECE);

  TIM3->CR1 |= (TIM_CR1_CEN);
}

static void Loop()
{
  uint32_t Frequency = TIM3->CNT;
  TIM3->CNT = 0;
  GPIOA->ODR ^= GPIO_ODR_OD5;
}


void ColorDetector_Init()
{
  GpioInit();
  TimerInit();
  Task task = {.Func = Loop, .Period = 1000, .Prioryty = 1};
  TaskMenager_AddTask(task);
}

static void SetColorFilter(ColorFilter Filter)
{
  switch(Filter)
    {
    case ColorFilter_None:
      SetStateS2(PinState_High);
      SetStateS3(PinState_Low);
      break;
    case ColorFilter_Red:
      SetStateS2(PinState_Low);
      SetStateS3(PinState_Low);
      break;
    case ColorFilter_Green:
      SetStateS2(PinState_High);
      SetStateS3(PinState_High);
      break;
    case ColorFilter_Blue:
      SetStateS2(PinState_Low);
      SetStateS3(PinState_High);
      break;
    default:
      break;
    }
}

static void SetPresacler(Prescaler prescaler)
{
  switch(prescaler)
    {
    case Prescaler_0:
      SetStateS0(PinState_Low);
      SetStateS1(PinState_Low);
      break;
    case Prescaler_2:
      SetStateS0(PinState_Low);
      SetStateS1(PinState_High);
      break;
    case Prescaler_20:
      SetStateS0(PinState_High);
      SetStateS1(PinState_Low);
      break;
    case Prescaler_100:
      SetStateS0(PinState_High);
      SetStateS1(PinState_High);
      break;
    default:
      break;
    }
}
