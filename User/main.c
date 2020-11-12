#include "main.h"

int a=0,b=0,c=0;
int aaa=0;
int speed_keyboarda(void);
int speed_keyboardb(void);
void bodan(void)
{
	if(Switch_Right == 1)
	{
		PWM3 = 3000;
		PI6_0;
		PI7_1;
	}
	else
	{
		PWM3 = 0;
		PI6_0;
		PI7_0;
	}
}

void bodan_mouse(void)
{
	if(Mouse_value_l)
	{
		PWM3 = SHOOT_FRE;
		PI6_0;
		PI7_1;
	}
	else if(Mouse_value_r)
	{
		PWM3 = SHOOT_FRE;
		PI6_0;
		PI7_1;
	}
	else if(Key_ctrl)
	{
		PWM3 = SHOOT_FRE;
		PI6_1;
		PI7_0;
	}
	else
	{
		PWM3=0;
	}
}
void check_gy901(void)
{
	static float gy901_angle = 0,speed_pitch = 0;
	static u8 count = 0,times = 0;
	
	count++;

		if(count > 50)
		{
			if((gy901_angle == angle_pitch)&&(w_y == speed_pitch))
			{
				delay_ms(40);
				if((gy901_angle == angle_pitch)&&(w_y == speed_pitch))
				{
					times++;
					if(times > 3)
						error_gy901 = 1 ;
				}
				else
					times = 0;
			}
			else
			{
					error_gy901 = 0 ;
			}
			gy901_angle = angle_pitch;
			speed_pitch = w_y;
			count = 0;
		}
		//printf("error_gy901=%d,,,,gy901_angle=%.1f,,,,,speed_pitch=%.1f\r\n",error_gy901,gy901_angle,speed_pitch);
}

int main(void)
{
	
USART2_Configuration();
	  //gy901数据接收
	delay_ms(1000);
	
	LED_Configuration();
	USART3_Configuration();  //串口数据接收
	USART2_Configuration();
	  
	PWM_Configuration();
	TIM6_Configuration();
	delay_ms(100);
	TIM6_Start();
	//检验初始化是否成功 失败重新初始化化
	
		USART1_Configuration();
	/**/
	while(1)
	{
			Radio_Judge();
		LEDred_ON();
		if(Switch_Left ==3)
		{
			LEDgreen_ON();
			delay_ms(3000);
			if(Switch_Left ==3)
				break;
		}
	}
	LEDgreen_OFF();
	LEDred_OFF();
	
		CAN1_Configuration();    
	CAN2_Configuration();
		while(1) 
		{
			
			//-----------------------------云台初始处理	
			
					if((angle_pitch > 19||angle_pitch < -40)&&(!error_gy901))//!first||
			{
				if(angle_pitch > 19)
				{
					while(!(((-6.5)-8<angle_pitch)&&(angle_pitch<(-6.5)+8)))
					target = angle_pitch-13;
				}
				if(angle_pitch < -40)
				{
					while(!(((-6.5)-8<angle_pitch)&&(angle_pitch<(-6.5)+8)))
					target = angle_pitch+13;	
				}
				//pitch_target_angle = pitch_current_angle;
				if(((-6.5)-5<angle_pitch)&&(angle_pitch<(-6.5)+5))
				{	
					target=-6.5;
					/*
				if(Push_Right_UAD == 0)
						first_times++;
					else
						first_times = 0;
					if(first_times > 30)
						first = 1;
					*/
				}
			}
			//------------------------------------------
			Radio_Judge();
			if(Push_Right_UAD <= -700 || Push_Right_UAD >= 700) Push_Right_UAD = 0;
			if(Push_Left_UAD <= -700 || Push_Left_UAD >= 700) Push_Left_UAD = 0;
			target_sign = Push_Right_UAD;
//---------------------------遥控器  键盘鼠标切换		
	
		 if(Switch_Left!=2)		/*&&Push_Right_UAD!=-1024&&Push_Left_LAR!=-1024&&!=-1024*/ 
		{
				if(pitch_current_angle > CNOMTROL_MAX ) pitch_current_angle = CNOMTROL_MAX;
				if(pitch_current_angle < CNOMTROL_MIN ) pitch_current_angle = CNOMTROL_MIN;
			if(Switch_Left==3||error_gy901)
			{
				if(((pitch_current_angle > CNOMTROL_MIN)&&(Push_Right_UAD > 0))||((pitch_current_angle < CNOMTROL_MAX)&&(Push_Right_UAD < 0)))
				{//切换
				//target += Push_Right_UAD  / 500.0;
					pitch_target_angle -= Push_Right_UAD / 45.0;
					
				}
			}
			else
			{
				if(((pitch_current_angle > CNOMTROL_MIN)&&(Push_Right_UAD > 0))||((pitch_current_angle < CNOMTROL_MAX)&&(Push_Right_UAD < 0)))
				{//不切换
					target -= Push_Right_UAD  / 2000.0;
				}
			}
			
				a=Push_Left_UAD*10;
				c=-Push_Right_LAR*10;
				b=-Push_Left_LAR*10;	 
				bodan();
		}
		else
		{
			if(((pitch_current_angle > CNOMTROL_MIN)&&(Mouse_value_y < 0))||((pitch_current_angle < CNOMTROL_MAX)&&(Mouse_value_y > 0)))
				target += Mouse_value_y / 120.0;
				
				a = speed_keyboarda()*3;
				b = speed_keyboardb()*3;
				c = -Mouse_value_x * 70 + Key_q * 3500 - Key_e * 3500;
				bodan_mouse();
		}

		aaa++;
			if(aaa > 20)
			{
				//printf("Gyro:%.3f %.3f %.3f\r\n",(,(float)stcGyro.w[1]/32768*2000,(float)stcGyro.w[2]/32768*2000);
				//printf("Angle:%.3f pitch==%.3f %.3f\r\n",(float)stcAngle.Angle[0]/32768*180,(float)stcAngle.Angle[1]/32768*180,(float)stcAngle.Angle[2]/32768*180);
				printf("pitch==%.1f---roll==%.1f---yaw==%.1f---w_y==%f>>>speed=%.1f\r\n",angle_pitch,angle_roll,angle_yaw,w_y,Pitch_speed);
			//	printf("%d+++%d+++%d+++%d	\r\n",motor201_target_velocity,motor202_target_velocity,motor203_target_velocity,motor204_target_velocity);
				printf("(%.1f>>>>%.1d>>>>%.1f )%.1d %.1d \r\n",angle_pitch,pitch_current,target,Switch_Left,Switch_Right);
				printf("<<%.1d^^^^^^%.1d.....%.1f>> (%.1d>>>>%.1d)\r\n",Push_Left_UAD,Push_Right_UAD,target_sign,pitch_current_angle,pitch_target_angle);
				printf("         error_gy901==%d                *******%.1d>>>>%.1d>>>>%.1d********* \r\n",error_gy901,yaw_current_angle,yaw_current,yaw_target_angle);
				aaa = 0;
			}
//-------------------------------gy901自检
			check_gy901();
			
			delay_ms(4);
						
	
//-------------------------------can1底盘			
		//motor_Set(a+b-c,-a+b-c,-(a+b+c),a-b-c);
		}
}

int speed_keyboarda(void)
{
	static int key = 0;
	if(Key_s)
	{
		key -= 12;
		if(key < -1500)
			key = -1500;
	}
	else if(Key_w)
	{
		key += 22;
		if(key > 1500)
			key = 1500;
	}
	else
	{
		if(key > 0)
		{
			key -= 100;
			if(key <= 0)
				key = 0;
		}
		else if(key < 0)
		{
			key += 100;
			if(key >= 0)
				key = 0;
		}
	}
	return key;
}
int speed_keyboardb(void)
{
	static int key = 0;
	if(Key_d)
	{
		key -= 24;
		if(key < -1300)
			key = -1300;
	}
	else if(Key_a)
	{
		key += 24;
		if(key > 1300)
			key = 1300;
	}
	else
	{
		if(key > 0)
		{
			key -= 150;
			if(key <= 0)
				key = 0;
		}
		else if(key < 0)
		{
			key += 150;
			if(key >= 0)
				key = 0;
		}
	}
	return key;
}
