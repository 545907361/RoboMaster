#ifndef __USART1_H__
#define __USART1_H__

#include "main.h"

extern unsigned char sbus_rx_buffer[18];
extern int  sbus_channel_temp[17];  //  temp sbus decode channel data

void USART1_Configuration(void);
void Radio_Judge(void);

extern u8   remote_flag ;
extern u8   Key_w,Key_s,Key_a,Key_d,Key_shift,Key_ctrl,Key_q,Key_e;
extern s16  Mouse_value_l,Mouse_value_r,Mouse_value_x,Mouse_value_y;
extern s16  Push_Left_UAD,Push_Left_LAR,Push_Right_UAD,Push_Right_LAR;
extern u8	  Switch_Left,Switch_Right;

extern float target_sign;   
extern u8 count_num;        
extern u8 sign;
extern float target_bias; 
#endif
