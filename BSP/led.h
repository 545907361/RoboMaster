#ifndef __LED_H__
#define __LED_H__

#include <stm32f4xx.h>


void LED_Configuration(void);


#define  LEDgreen_ON()        GPIO_ResetBits(GPIOF,GPIO_Pin_14)
#define  LEDgreen_OFF()       GPIO_SetBits(GPIOF,GPIO_Pin_14)
#define  LEDgreen_TOGGLE()    GPIO_ToggleBits(GPIOF,GPIO_Pin_14)

#define  LEDred_OFF()        GPIO_SetBits(GPIOE,GPIO_Pin_7) 
#define  LEDred_ON()        GPIO_ResetBits(GPIOE,GPIO_Pin_7)

#endif

