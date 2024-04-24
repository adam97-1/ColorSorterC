#include "MotorSel.h"
#include <stm32f446xx.h>
#include <stdint.h>
#include "TaskMenager.h"
#include "EncoderSel.h"

//IN2 -- PB6/TIM4_CH1
//IN1 -- PA7/TIM14_CH1

static uint32_t maxPwm = 2000;
static float maxSpeed = 15.6f;

static uint32_t period = 1;

static float targetPosition = 3.f;

static float sumPosition= 0.f;
static float sumSpeed = 0.f;
static float oldPosition = 0.f;
static float oldSpeed = 0.f;

static float Kp_Position = 1.2f;
static float Ki_Position = 0.2f;
static float Kd_Position = 0.05f;
static float saturationPosition = 3.5f;

static float Kp_Speed = 0.8f;
static float Ki_Speed = 0.0f;
static float Kd_Speed = 0.08f;
static float saturationSpeed = 3.5f;

static void Loop()
{

  float position = EncoderSel_GetPosition();

  sumPosition += (targetPosition - position)/(period*0.001);
  float sumSaturtionPosition = Ki_Position*sumPosition;
  if(sumSaturtionPosition > saturationPosition)
	  sumSaturtionPosition = saturationPosition;
  else if(sumSaturtionPosition < -saturationPosition)
	  sumSaturtionPosition = -saturationPosition;

  float dPosition= (position - oldPosition)/(period*0.001);

  float diffPosition = targetPosition - position;

  float targetSpeed = Kp_Position*diffPosition + sumSaturtionPosition + Kd_Position*dPosition;


  float speed = EncoderSel_GetSpeed();

  sumSpeed += (targetSpeed - speed)/(period*0.001);
  float sumSaturtionSpeed = Ki_Speed*sumSpeed;
  if(sumSaturtionSpeed > saturationSpeed)
	  sumSaturtionSpeed = saturationSpeed;
  else if(sumSaturtionSpeed < -saturationSpeed)
	  sumSaturtionSpeed = -saturationSpeed;

  float dSpeed = (speed - oldSpeed)/period;

  float diffSpeed= targetSpeed - speed;

  float newSpeed = Kp_Speed*diffSpeed + sumSaturtionSpeed + Kd_Speed*dSpeed;

  int32_t PWM = newSpeed*maxPwm/maxSpeed;
  if(newSpeed > 0)
  {
	  if (PWM > maxPwm)
		  PWM = maxPwm;
	  TIM14->CCR1 = 0;
	  TIM4->CCR1 = PWM;
  }
  else
  {
	  PWM *= -1;
	  if (PWM > maxPwm)
		  PWM = maxPwm;
	  TIM4->CCR1 = 0;
	  TIM14->CCR1 = PWM;
  }
  oldPosition = position;
  oldSpeed = speed;
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
