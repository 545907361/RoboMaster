#include "main.h"

int Pwm;
extern u8 Radio_switchl,Radio_switchr;


void TIM6_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  tim;
    NVIC_InitTypeDef         nvic;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
    
    nvic.NVIC_IRQChannel = TIM6_DAC_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    tim.TIM_Prescaler = 42-1;
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    tim.TIM_Period = 300000;
    TIM_TimeBaseInit(TIM6,&tim);
}

void TIM6_Start(void)
{
		Pwm=1000;
    TIM_Cmd(TIM6, ENABLE);	 
    TIM_ITConfig(TIM6, TIM_IT_Update,ENABLE);
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);	
}

void TIM6_DAC_IRQHandler(void)  
{
    if (TIM_GetITStatus(TIM6,TIM_IT_Update)!= RESET) 
	{
        TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
        TIM_ClearFlag(TIM6, TIM_FLAG_Update);
  }
		if(Radio_switchr==1|Radio_switchr==2)
		{

			Pwm+=5;
			PWM1=Pwm;
			PWM2=Pwm;
			if(Pwm>2400) 
			Pwm=2400;
	 }
	 	else
	{
			PWM1=800;
			PWM2=800;
	}

}
