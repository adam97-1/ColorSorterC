#include "MotorSel/MotorSel.h"
#include <stm32f446xx.h>
#include <stdint.h>
#include <math.h>
#include "TaskMenager.h"
#include "MotorSel/EncoderSel.h"

//IN2 -- PB6/TIM4_CH1
//IN1 -- PA7/TIM14_CH1

static float maxPwm = 2000;
static float maxSpeed = 15.6f;
static float maxPosition = 2*M_PI;

static uint32_t period = 1;

static float targetPosition = 3.f;

static float sumPosition= 0.f;
static float sumSpeed = 0.f;
static float oldPosition = 0.f;
static float oldSpeed = 0.f;

static float Kp_Position = 5.f;
static float Ki_Position = 0.0f;
static float Kd_Position = 0.0f;
static float saturationPosition = 100000.f;

static float Kp_Speed = 300.f;
static float Ki_Speed = 10.0f;
static float Kd_Speed = 0.0f;
static float saturationSpeed = 1000000.f;

static float outPositionDeb =0;
float outDeb2 = 0;
float pos_err_calc_up(float akt_pos, float exp_pos)
{

	float distance = 0;

	if(akt_pos >= exp_pos)
	{
		distance =  2*M_PI - akt_pos + exp_pos;
	}
	else
	{
		distance = exp_pos - akt_pos;
	}

	return distance;
}

float pos_err_calc_dwn(float akt_pos, float exp_pos)
{

	float distance = 0;

	if(akt_pos >= exp_pos)
	{
		distance =   akt_pos - exp_pos;
	}
	else
	{
		distance = 2*M_PI - exp_pos + akt_pos;
	}

	return distance;
}

float pos_err_calc(float akt_pos, float exp_pos)
{

	float distance = 0;
	float step1, step2;

	step1 = pos_err_calc_up(akt_pos, exp_pos);
	step2 = pos_err_calc_dwn(akt_pos, exp_pos);
	if(step1<step2)
		distance = step1;
	else
		distance = -step2;

	return distance;
}

static void Loop()
{

	float position = EncoderSel_GetPosition();

	if(targetPosition > maxPosition)
		targetPosition = maxPosition;
	else if(targetPosition < 0)
		targetPosition = 0;

	float position_error = pos_err_calc( position, targetPosition);

	sumPosition += position_error;//  /(period*0.0001);
	if(sumPosition > saturationPosition)
		sumPosition = saturationPosition;
	else if(sumPosition < -saturationPosition)
		sumPosition = -saturationPosition;

	float outPosition = 	Kp_Position * position_error +
							Ki_Position * sumPosition +
							Kd_Position * (position - oldPosition);
							//(period*0.001);*/

	float speed = -EncoderSel_GetSpeed();

	if(outPosition > maxSpeed)
	{
		outPosition = maxSpeed;
		sumPosition -= position_error;
	}
	else if(outPosition < -maxSpeed)
	{
		outPosition = -maxSpeed;
		sumPosition -= position_error;
	}

	float speed_error = outPosition - speed;

	sumSpeed += speed_error;//  /(period*0.0001);
	if(sumSpeed > saturationSpeed)
		sumSpeed = saturationSpeed;
	else if(sumSpeed < -saturationSpeed)
	sumSpeed = -saturationSpeed;

	float out = 		Kp_Speed * speed_error +
						Ki_Speed * sumSpeed +
						Kd_Speed * (speed - oldSpeed);
//						(period*0.001);

	if(out > maxPwm)
	{
		out = maxPwm;
		//sumPosition -= position_error;
		sumSpeed -= speed_error;
	}
	else if(out < -maxPwm)
	{
		out = -maxPwm;
		//sumPosition -= position_error;
		sumSpeed -= speed_error;
	}

	outDeb2 = out;

	if(out > 0)
	{
		TIM14->CCR1 = 0;
		TIM4->CCR1 = (uint32_t)out;
	}
	else
	{
		TIM14->CCR1 = (uint32_t)-out;
		TIM4->CCR1 = 0;
	}
	oldPosition= position;
	oldSpeed = speed;

	outPositionDeb = outPosition;

}

void MotorSel_Init()
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;


  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN | RCC_APB1ENR_TIM14EN;

  GPIOA->MODER &= ~(GPIO_MODER_MODE7_Msk);
  GPIOA->MODER |= (2 << GPIO_MODER_MODE7_Pos);

  GPIOB->MODER &= ~(GPIO_MODER_MODE6_Msk);
  GPIOB->MODER |= (2 << GPIO_MODER_MODE6_Pos);

  GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD7_Msk     );
  GPIOA->PUPDR |= (2 << GPIO_PUPDR_PUPD7_Pos );

  GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD6_Msk     );
  GPIOB->PUPDR |= (2 << GPIO_PUPDR_PUPD6_Pos );

  GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL7_Msk     );
  GPIOA->AFR[0] |= (0b1001 << GPIO_AFRL_AFSEL7_Pos );

  GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL6_Msk     );
  GPIOB->AFR[0] |= (2 << GPIO_AFRL_AFSEL6_Pos );

  TIM4->CCMR1 &= ~(TIM_CCMR1_OC1M_Msk);
  TIM4->CCMR1 |= (0b111 << TIM_CCMR1_OC1M_Pos | TIM_CCMR1_OC1PE);

  TIM14->CCMR1 &= ~(TIM_CCMR1_OC1M_Msk);
  TIM14->CCMR1 |= (0b111 << TIM_CCMR1_OC1M_Pos | TIM_CCMR1_OC1PE);

  TIM4->CCER |= (TIM_CCER_CC1E);

  TIM14->CCER |= (TIM_CCER_CC1E);

  TIM4->ARR = maxPwm;
  TIM14->ARR = maxPwm;

  TIM4->CR1 |= (TIM_CR1_CEN);
  TIM14->CR1 |= (TIM_CR1_CEN);

  Task task = {.Func = Loop, .Period = period, .Prioryty = 1};
  TaskMenager_AddTask(task);
}
