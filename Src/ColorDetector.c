#include "ColorDetector.h"
#include <stm32f446xx.h>
#include <stddef.h>
#include "TaskMenager.h"

// S0  - PC6
// S1  - PC5
// S2  - PC10
// S3  - PC11
// LED - PC12
// OUT - PD2/TIM3_ETR

typedef enum {
	ColorDetector_ColorFilter_None,
	ColorDetector_ColorFilter_Red,
	ColorDetector_ColorFilter_Green,
	ColorDetector_ColorFilter_Blue
} ColorDetector_ColorFilter;

typedef enum {
	ColorDetector_Prescaler_0,
	ColorDetector_Prescaler_2,
	ColorDetector_Prescaler_20,
	ColorDetector_Prescaler_100
} ColorDetector_Prescaler;

static inline void SetStateS0(ColorDetector_PinState state);
static inline void SetStateS1(ColorDetector_PinState state);
static inline void SetStateS2(ColorDetector_PinState state);
static inline void SetStateS3(ColorDetector_PinState state);
static inline void SetStateLed(ColorDetector_PinState state);
static inline void GpioInit();
static inline void TimerInit();
static void Loop();
static void SetColorFilter(ColorDetector_ColorFilter filter);
static void SetPrescaler(ColorDetector_Prescaler prescaler);
static void ColorReady(ColorDetector_Color color);

static void (*m_colorReadyFunc)(ColorDetector_Color color);
static ColorDetector_Color m_color = { .Red = 0, .Green = 0, .Blue = 0 };
static bool m_isColorReady = false;
static bool m_isColorMeasurment = false;
static uint32_t m_msPeriod = 300;

static inline void SetStateS0(ColorDetector_PinState state)
{
	switch (state)
	{
	case ColorDetector_PinState_High:
		GPIOC->ODR |= GPIO_ODR_OD6;
		break;
	case ColorDetector_PinState_Low:
		GPIOC->ODR &= ~GPIO_ODR_OD6;
		break;
	default:
		break;
	}
}

static inline void SetStateS1(ColorDetector_PinState state)
{
	switch (state)
	{
	case ColorDetector_PinState_High:
		GPIOC->ODR |= GPIO_ODR_OD5;
		break;
	case ColorDetector_PinState_Low:
		GPIOC->ODR &= ~GPIO_ODR_OD5;
		break;
	default:
		break;
	}
}
static inline void SetStateS2(ColorDetector_PinState state)
{
	switch (state)
	{
	case ColorDetector_PinState_High:
		GPIOC->ODR |= GPIO_ODR_OD10;
		break;
	case ColorDetector_PinState_Low:
		GPIOC->ODR &= ~GPIO_ODR_OD10;
		break;
	default:
		break;
	}
}
static inline void SetStateS3(ColorDetector_PinState state)
{
	switch (state)
	{
	case ColorDetector_PinState_High:
		GPIOC->ODR |= GPIO_ODR_OD11;
		break;
	case ColorDetector_PinState_Low:
		GPIOC->ODR &= ~GPIO_ODR_OD11;
		break;
	default:
		break;
	}
}

static inline void SetStateLed(ColorDetector_PinState state)
{
	switch (state)
	{
	case ColorDetector_PinState_High:
		GPIOC->ODR |= GPIO_ODR_OD12;
		break;
	case ColorDetector_PinState_Low:
		GPIOC->ODR &= ~GPIO_ODR_OD12;
		break;
	default:
		break;
	}
}

static inline void GpioInit()
{
	RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOCEN);

	GPIOC->MODER &= ~(GPIO_MODER_MODE5_Msk 		| GPIO_MODER_MODE6_Msk		| GPIO_MODER_MODE10_Msk 	|
					GPIO_MODER_MODE11_Msk 		| GPIO_MODER_MODE12_Msk);
	GPIOC->MODER |= (1 << GPIO_MODER_MODE5_Pos 	| 1 << GPIO_MODER_MODE6_Pos | 1 << GPIO_MODER_MODE10_Pos |
					1 << GPIO_MODER_MODE11_Pos	| 1 << GPIO_MODER_MODE12_Pos);

	GPIOD->MODER &= ~(GPIO_MODER_MODE2_Msk);
	GPIOD->MODER |= (2 << GPIO_MODER_MODE2_Pos);

	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT5 	| GPIO_OTYPER_OT6 	|
					GPIO_OTYPER_OT10	| GPIO_OTYPER_OT11 	|
					GPIO_OTYPER_OT12);

	GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED5_Msk 		| GPIO_OSPEEDR_OSPEED6_Msk 		|
						GPIO_OSPEEDR_OSPEED10_Msk 		| GPIO_OSPEEDR_OSPEED11_Msk 	|
						GPIO_OSPEEDR_OSPEED12_Msk);
	GPIOC->OSPEEDR |= 	(0 << GPIO_OSPEEDR_OSPEED5_Pos 	| 0 << GPIO_OSPEEDR_OSPEED6_Pos 	|
						0 << GPIO_OSPEEDR_OSPEED10_Pos	| 0 << GPIO_OSPEEDR_OSPEED11_Pos	|
						0 << GPIO_OSPEEDR_OSPEED12_Pos);

	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD5_Msk 		| GPIO_PUPDR_PUPD6_Msk		|
					GPIO_PUPDR_PUPD10_Msk 		| GPIO_PUPDR_PUPD11_Msk 	|
					GPIO_PUPDR_PUPD12_Msk);
	GPIOC->PUPDR |= (1 << GPIO_PUPDR_PUPD5_Pos 	| 1 << GPIO_PUPDR_PUPD6_Pos	|
					1 << GPIO_PUPDR_PUPD10_Pos 	| 1 << GPIO_PUPDR_PUPD11_Pos|
					1 << GPIO_PUPDR_PUPD12_Pos);
}

static inline void TimerInit()
{
	RCC->AHB1ENR |= (RCC_AHB1ENR_GPIODEN);
	RCC->APB1ENR |= (RCC_APB1ENR_TIM3EN);

	GPIOD->MODER &= ~(GPIO_MODER_MODE2_Msk);
	GPIOD->MODER |= (2 << GPIO_MODER_MODE2_Pos);

	GPIOD->AFR[0] &= ~(GPIO_AFRL_AFSEL2_Msk);
	GPIOD->AFR[0] |= (2 << GPIO_AFRL_AFSEL2_Pos);

	TIM3->SMCR &= ~(TIM_SMCR_ETF_Msk 		| TIM_SMCR_ETPS_Msk 		| TIM_SMCR_ETP);
	TIM3->SMCR |= (0 << TIM_SMCR_ETF_Pos 	| 1 << TIM_SMCR_ETPS_Pos 	| TIM_SMCR_ECE);

	TIM3->CR1 |= (TIM_CR1_CEN);
}

static void Loop()
{
	static uint8_t subColorMeasurement = 0;

	if (m_isColorMeasurment == false)
		return;

	uint32_t frequency = TIM3->CNT / 2;
	TIM3->CNT = 0;

	switch (subColorMeasurement)
	{
	case 0:
		m_color.Red = frequency;
		SetColorFilter(ColorDetector_ColorFilter_Green);
		break;
	case 1:
		m_color.Blue = frequency;
		SetColorFilter(ColorDetector_ColorFilter_Blue);
		break;
	case 2:
		m_color.Green = frequency;
		SetColorFilter(ColorDetector_ColorFilter_Red);
		SetStateLed(ColorDetector_PinState_Low);
		m_isColorReady = true;
		m_isColorMeasurment = false;
		ColorReady(m_color);
		break;
	default:
		break;
	}
	subColorMeasurement++;
	subColorMeasurement %= 3;
}

void ColorDetector_Init(uint32_t msPeriod )
{
	m_msPeriod  = msPeriod;

	GpioInit();
	TimerInit();
	SetColorFilter(ColorDetector_ColorFilter_Red);
	SetPrescaler(ColorDetector_Prescaler_100);
	Task task = { .Func = Loop, .Period = m_msPeriod, .Prioryty = 1 };
	TaskMenager_AddTask(task);
}

static void SetColorFilter(ColorDetector_ColorFilter filter)
{
	switch (filter) {
	case ColorDetector_ColorFilter_None:
		SetStateS2(ColorDetector_PinState_High);
		SetStateS3(ColorDetector_PinState_Low);
		break;
	case ColorDetector_ColorFilter_Red:
		SetStateS2(ColorDetector_PinState_Low);
		SetStateS3(ColorDetector_PinState_Low);
		break;
	case ColorDetector_ColorFilter_Green:
		SetStateS2(ColorDetector_PinState_High);
		SetStateS3(ColorDetector_PinState_High);
		break;
	case ColorDetector_ColorFilter_Blue:
		SetStateS2(ColorDetector_PinState_Low);
		SetStateS3(ColorDetector_PinState_High);
		break;
	default:
		break;
	}
}

static void SetPrescaler(ColorDetector_Prescaler prescaler)
{
	switch (prescaler) {
	case ColorDetector_Prescaler_0:
		SetStateS0(ColorDetector_PinState_Low);
		SetStateS1(ColorDetector_PinState_Low);
		break;
	case ColorDetector_Prescaler_2:
		SetStateS0(ColorDetector_PinState_Low);
		SetStateS1(ColorDetector_PinState_High);
		break;
	case ColorDetector_Prescaler_20:
		SetStateS0(ColorDetector_PinState_High);
		SetStateS1(ColorDetector_PinState_Low);
		break;
	case ColorDetector_Prescaler_100:
		SetStateS0(ColorDetector_PinState_High);
		SetStateS1(ColorDetector_PinState_High);
		break;
	default:
		break;
	}
}

const ColorDetector_Color* ColorDetector_GetColor()
{
	m_isColorReady = false;
	m_isColorMeasurment = true;
	SetStateLed(ColorDetector_PinState_High);

	return &m_color;
}

bool ColorDetector_IsColorReady()
{
	return m_isColorReady;
}

void ColorDetector_SetColorReadyFunc(void (*func)(ColorDetector_Color color))
{
	m_colorReadyFunc = func;
}

static void ColorReady(ColorDetector_Color color)
{
	if (m_colorReadyFunc == NULL)
		return;
	m_colorReadyFunc(color);
}
