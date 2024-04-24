#include "MotorDriv.h"
#include <stm32f446xx.h>
#include <stdint.h>
#include "TaskMenager.h"
#include "EncoderDriv.h"

//IN2 -- PB8/TIM4_CH3
//IN1 -- PB9/TIM4_CH4

static uint32_t maxPwm = 2000;
static float maxSpeed = 15.6f;

static uint32_t period = 1;

static float targetSpeed = 3.f;

static float sumSpeed = 0.f;
static float oldSpeed = 0.f;

static float Kp_Speed = 0.8f;
static float Ki_Speed = 0.0f;
static float Kd_Speed = 0.08f;
static float saturationSpeed = 3.5f;
  
static void Loop()
{
	float speed = EncoderDriv_GetSpeed();

	  sumSpeed += (targetSpeed - speed)/(period*0.001);
	  float sumSaturtionSpeed = Ki_Speed*sumSpeed;
	  if(sumSaturtionSpeed > saturationSpeed)
		  sumSaturtionSpeed = saturationSpeed;
	  else if(sumSaturtionSpeed < -saturationSpeed)
		  sumSaturtionSpeed = -saturationSpeed;

	  float dSpeed = (speed - oldSpeed)/period;

	  float diffSpeed = targetSpeed - speed;

	  float newSpeed = Kp_Speed*diffSpeed + sumSaturtionSpeed + Kd_Speed*dSpeed;

	  int32_t PWM = newSpeed*maxPwm/maxSpeed;
	  if(newSpeed > 0)
	  {
		  if (PWM > maxPwm)
			  PWM = maxPwm;
		  TIM4->CCR3 = 0;
		  TIM4->CCR4 = PWM;
	  }
	  else
	  {
		  PWM *= -1;
		  if (PWM > maxPwm)
			  PWM = maxPwm;
		  TIM4->CCR3 = PWM;
		  TIM4->CCR4 = 0;
	  }
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

  Task task = {.Func = Loop, .Period = 10, .Prioryty = 1};
  TaskMenager_AddTask(task);
}
