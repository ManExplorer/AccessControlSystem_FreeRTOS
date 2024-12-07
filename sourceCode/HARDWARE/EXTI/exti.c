#include "led.h"
#include "key.h"
#include "delay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#define EVENTBIT_0 (1<<0)
  
//外部中断0服务程序
void EXTIX_Init(void)
{
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	KEY_Init();	 //	按键端口初始化

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

 //GPIOE.4	  中断线以及中断初始化配置  下降沿触发	//KEY0
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);

	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;				//使能按键KEY0所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x06;	//抢占优先级6
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;			//子优先级0 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

//任务句柄
extern EventGroupHandle_t EventGroupHandler;

//外部中断4服务程序
void EXTI4_IRQHandler(void)
{
	BaseType_t Result,xHigherPriorityTaskWoken;
	
	delay_xms(50);	//消抖
	
	if(KEY0==0)	 
	{				 
		Result=xEventGroupSetBitsFromISR(EventGroupHandler,EVENTBIT_0,&xHigherPriorityTaskWoken);//通过中断来设置标志位
		
		if(Result!=pdFAIL)
		{
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//调用该函数进行上下文切换
		}
	}		
	
	 EXTI_ClearITPendingBit(EXTI_Line4);//清除LINE4上的中断标志位  
}


