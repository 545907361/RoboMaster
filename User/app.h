#ifndef __APP_H__
#define __APP_H__
//#include <stm32f4xx.h>

#define abs(x) ((x)>0? (x):(-(x)))

void Cmd_ESC(int16_t current_201,int16_t current_202);
void RM3510_Current_Set(int16_t current_motor1,int16_t current_motor2,int16_t current_motor3,int16_t current_motor4);


#endif
