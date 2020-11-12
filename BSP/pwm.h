#ifndef __PWM_H__
#define __PWM_H__

void PWM_Configuration(void);

#define SHOOT_FRE    1900  //ÉäÆµ
#define PWM1  TIM12->CCR1
#define PWM2  TIM12->CCR2
#define PWM3  TIM2->CCR1

#define     PI6_1        GPIO_SetBits(GPIOI,GPIO_Pin_6)
#define     PI6_0        GPIO_ResetBits(GPIOI,GPIO_Pin_6)

#define     PI7_1        GPIO_SetBits(GPIOI,GPIO_Pin_7)
#define     PI7_0        GPIO_ResetBits(GPIOI,GPIO_Pin_7)

#endif


