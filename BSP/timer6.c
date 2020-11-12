#include "main.h"

int Pwm;


void TIM6_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  tim;
    NVIC_InitTypeDef         nvic;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
    
    nvic.NVIC_IRQChannel = TIM6_DAC_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;
    nvic.NVIC_IRQChannelSubPriority = 2;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    tim.TIM_Prescaler = 84-1;
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
void moca(void)
{
	static int num = 0;
	static u8 sign = 0;
	if((Switch_Right == 2)&&((num - Switch_Right) == 1))//-----------------
	{
		if(sign)
			sign = 0;
		else
			sign = 1;
	}
	if(sign)
	{
		Pwm+=2.8;
		PWM1=Pwm;
		PWM2=Pwm;
		if(Pwm>1450) 
		Pwm=1450;
	}
	else 
	{
		PWM1=800;
		PWM2=800;
	}
	num = Switch_Right;              //----------------------------
}	
void TIM6_DAC_IRQHandler(void)  
{
    if (TIM_GetITStatus(TIM6,TIM_IT_Update)!= RESET) 
	{
        TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
        TIM_ClearFlag(TIM6, TIM_FLAG_Update);
  }
	moca();

}
