#include "main.h"
/*-LEFT---(PB3---TIM2_CH2)--*/
/*-RIGHT--(PA15--TIM2_CH1)--*/

void PWM_Configuration(void)
{
    GPIO_InitTypeDef          gpio;
    TIM_TimeBaseInitTypeDef   tim;
    TIM_OCInitTypeDef         oc;
    
	
	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI,ENABLE);
		gpio.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;      
    gpio.GPIO_Mode = GPIO_Mode_OUT;          
    gpio.GPIO_Speed = GPIO_Speed_50MHz;      
    GPIO_Init(GPIOI,&gpio);
		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
		
		gpio.GPIO_Pin =GPIO_Pin_0;
    gpio.GPIO_Mode = GPIO_Mode_AF;
		//gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
		
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM2);
		GPIO_Init(GPIOA,&gpio);
		/*
		tim.TIM_Prescaler =84-1;
		tim.TIM_Period =2500-1;   //  2.5ms
		*/
		tim.TIM_Prescaler =168-1;
		tim.TIM_Period =10000-1;		//  84000000/168/10000=50Hz  20ms
		
		tim.TIM_CounterMode = TIM_CounterMode_Up;
		tim.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2,&tim);
		
		oc.TIM_OCMode = TIM_OCMode_PWM1;
    oc.TIM_OutputState = TIM_OutputState_Enable;
    oc.TIM_OutputNState = TIM_OutputState_Disable;
    oc.TIM_Pulse = 0;
    oc.TIM_OCPolarity = TIM_OCPolarity_High;
    oc.TIM_OCNPolarity = TIM_OCPolarity_Low;
    oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
    oc.TIM_OCNIdleState = TIM_OCIdleState_Set;
		TIM_OC1Init(TIM2,&oc);
		TIM_CtrlPWMOutputs(TIM2,ENABLE);
		TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM2,ENABLE);
    TIM_Cmd(TIM2,ENABLE);


		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);
	
	  gpio.GPIO_Pin =GPIO_Pin_9|GPIO_Pin_6;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;

	
    GPIO_PinAFConfig(GPIOH,GPIO_PinSource9, GPIO_AF_TIM12);
    GPIO_PinAFConfig(GPIOH,GPIO_PinSource6, GPIO_AF_TIM12);    
    GPIO_Init(GPIOH,&gpio);
    tim.TIM_Prescaler =84-1;
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_Period =2000-1;   //2.5ms
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM12,&tim);

		oc.TIM_OCMode = TIM_OCMode_PWM1;
    oc.TIM_OutputState = TIM_OutputState_Enable;
    oc.TIM_OutputNState = TIM_OutputState_Disable;
    oc.TIM_Pulse = 1000;
    oc.TIM_OCPolarity = TIM_OCPolarity_High;
    oc.TIM_OCNPolarity = TIM_OCPolarity_High;
    oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
    oc.TIM_OCNIdleState = TIM_OCIdleState_Set;
    TIM_OC1Init(TIM12,&oc);
    TIM_OC2Init(TIM12,&oc);
   
    TIM_OC1Init(TIM12,&oc);
    TIM_OC2Init(TIM12,&oc);
	
    TIM_OC1PreloadConfig(TIM12,TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM12,TIM_OCPreload_Enable);
	 
    TIM_ARRPreloadConfig(TIM12,ENABLE);
    
    TIM_CtrlPWMOutputs(TIM12,ENABLE);
    
    TIM_Cmd(TIM12,ENABLE);

}


