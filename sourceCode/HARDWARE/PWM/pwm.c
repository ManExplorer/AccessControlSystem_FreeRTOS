#include "pwm.h"

//#define PWM_PERIOD 20000    // PWM 周期（微秒）
//#define PWM_MIN_PULSE 1000   // 最小脉冲宽度
//#define PWM_MAX_PULSE 2000   // 最大脉冲宽度


//定时器2 PWM初始化
void TIM2_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  
	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
	

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
//CH1	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
//CH12
	TIM_OC2Init(TIM2, &TIM_OCInitStructure); 
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
//CH13
	TIM_OC3Init(TIM2, &TIM_OCInitStructure); 
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable); 
//CH14
	TIM_OC4Init(TIM2, &TIM_OCInitStructure); 
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM2, ENABLE); //使能定时器2
}

void set_Angle(float angle)
{
int servo_temp;

if(angle>180)
angle=180;
else if(angle<0)
angle=0;

//servo_temp=angle*4000/180+1000;
servo_temp=5*(angle/45)+5;//5-0°，10-45°，15-90°，20-135°，25-180°



TIM_SetCompare1(TIM2,servo_temp);
}