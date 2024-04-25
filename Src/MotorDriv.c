#include "MotorDriv.h"
#include <stm32f446xx.h>
#include <stdint.h>
#include "TaskMenager.h"
#include "EncoderDriv.h"

//IN2 -- PB8/TIM4_CH3
//IN1 -- PB9/TIM4_CH4

static float maxPwm = 2000;
static float maxSpeed = 15.6f;

static uint32_t period = 1;

static float targetSpeed = 10.f;

static float sumSpeed = 0.f;
static float oldSpeed = 0.f;

static float Kp_Speed = 300.f;
static float Ki_Speed = 10.f;
static float Kd_Speed = 0.f;
static float saturationSpeed = 100000.0f;
float outDeb = 0;
float speedDeb = 0;
float speederrorDeb = 0;

static void Loop2()
{
	float speed = EncoderDriv_GetSpeed();

	if(targetSpeed > maxSpeed)
		targetSpeed = maxSpeed;
	else if(targetSpeed < -maxSpeed)
		targetSpeed = -maxSpeed;

	float speed_error = targetSpeed - speed;

	sumSpeed += speed_error;//  /(period*0.0001);
	if(sumSpeed > saturationSpeed)
		sumSpeed = saturationSpeed;
	else if(sumSpeed < -saturationSpeed)
	sumSpeed = -saturationSpeed;

	float out = 	Kp_Speed * speed_error +
					Ki_Speed * sumSpeed +
					Kd_Speed * (speed - oldSpeed);
					//(period*0.001);

	if(out > maxPwm)
	{
		out = maxPwm;
		sumSpeed -= speed_error;
	}
	else if(out < -maxPwm)
	{
		out = -maxPwm;
		sumSpeed -= speed_error;
	}

	if(out > 0)
	{
		TIM4->CCR3 = 0;
		TIM4->CCR4 = (uint32_t)out;
	}
	else
	{
		TIM4->CCR3 = (uint32_t)-out;;
		TIM4->CCR4 = 0;
	}
	outDeb = out;
	speedDeb = speed;
	speederrorDeb = speed_error;
	oldSpeed = speed;
}

void MotorDriv_Init()
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

  GPIOB->MODER &= ~(GPIO_MODER_MODE8_Msk     | GPIO_MODER_MODE9_Msk);
  GPIOB->MODER |= (2 << GPIO_MODER_MODE8_Pos | 2 << GPIO_MODER_MODE9_Pos);
  

  GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD8_Msk     | GPIO_PUPDR_PUPD9_Msk);
  GPIOB->PUPDR |= (2 << GPIO_PUPDR_PUPD8_Pos | 2 << GPIO_PUPDR_PUPD9_Pos);

  GPIOB->AFR[1] &= ~(GPIO_AFRH_AFSEL8_Msk     | GPIO_AFRH_AFSEL9_Msk);
  GPIOB->AFR[1] |= (2 << GPIO_AFRH_AFSEL8_Pos | 2 << GPIO_AFRH_AFSEL9_Pos);

  TIM4->CCMR2 &= ~(TIM_CCMR2_OC3M_Msk         | TIM_CCMR2_OC4M_Msk);
  TIM4->CCMR2 |= (0b111 << TIM_CCMR2_OC3M_Pos | 0b111 << TIM_CCMR2_OC4M_Pos |
                  TIM_CCMR2_OC3PE             | TIM_CCMR2_OC4PE);
  
  TIM4->CCER |= (TIM_CCER_CC3E | TIM_CCER_CC4E);

  TIM4->ARR = maxPwm;

  TIM4->CR1 |= (TIM_CR1_CEN);

  Task task = {.Func = Loop2, .Period = period, .Prioryty = 1};
  TaskMenager_AddTask(task);
}
