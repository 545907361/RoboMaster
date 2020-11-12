#include "main.h"


/********************************************************************************
   给电调板发送指令，ID号为0x200，只用两个电调板，数据回传ID为0x201和0x202
	 cyq:更改为发送三个电调的指令。
*********************************************************************************/

//减速电机
//输入参数：对应电机号电流，范围-32768~32768

	 /*****************************  底盘PID  *************************************/
 float RM3510_ep = 6.0;//4，原来1.7 5 6
 float RM3510_ei = 0.01;//原来0.01
 float RM3510_ed = 0.0001;//原来0.0;//0.0001
 float RM3510_vp = 6;//4 5
 float RM3510_vi = 0.01;//0
 float RM3510_vd =0.0001;//
 float RM3510_x1 = 0.6;
 float RM3510_x2 = 1.0;
	 
s16 motor201_current_electric=0,motor201_target_electric=0,motor201_current_velocity=0,motor201_target_velocity=0,motor201_current=0;
s16 motor202_current_electric=0,motor202_target_electric=0,motor202_current_velocity=0,motor202_target_velocity=0,motor202_current=0;
s16 motor203_current_electric=0,motor203_target_electric=0,motor203_current_velocity=0,motor203_target_velocity=0,motor203_current=0;
s16 motor204_current_electric=0,motor204_target_electric=0,motor204_current_velocity=0,motor204_target_velocity=0,motor204_current=0;


	 
/********************************************************************************
                         RM3510总PID输出
*********************************************************************************/

void RM3510_PID1()
{
	static float speed;
	speed = RM3510_Velocity_Control_motor1(motor201_current_velocity,motor201_target_velocity);
	motor201_current_velocity = (speed * RM3510_x1 +RM3510_Electric_Control_motor1(motor201_current_electric,speed) * RM3510_x2);//加了负号
	motor201_current_electric=speed;
	if(motor201_current_velocity > tyre_ESC_MAX)
	motor201_current_velocity = tyre_ESC_MAX;
	
	if(motor201_current_velocity < -tyre_ESC_MAX)
	motor201_current_velocity = -tyre_ESC_MAX;	

}
void RM3510_PID2()
{
	static float speed;
	speed = RM3510_Velocity_Control_motor2(motor202_current_velocity,motor202_target_velocity);
	motor202_current_velocity = (speed * RM3510_x1 + RM3510_Electric_Control_motor2(motor202_current_electric,speed) * RM3510_x2);//加了负号
	motor202_current_electric=speed;
	if(motor202_current_velocity > tyre_ESC_MAX)	
	motor202_current_velocity = tyre_ESC_MAX;
	if(motor202_current_velocity < -tyre_ESC_MAX)
	motor202_current_velocity = -tyre_ESC_MAX;	
}
void RM3510_PID3()
{
	static float speed;
	speed = RM3510_Velocity_Control_motor3(motor203_current_velocity,motor203_target_velocity);
	motor203_current_velocity = (speed * RM3510_x1 + RM3510_Electric_Control_motor3(motor203_current_electric,speed) * RM3510_x2);//加了负号
	motor203_current_electric=speed;
	if(motor203_current_velocity > tyre_ESC_MAX)
	motor203_current_velocity = tyre_ESC_MAX;
	if(motor203_current_velocity < -tyre_ESC_MAX)
	motor203_current_velocity = -tyre_ESC_MAX;		
}
void RM3510_PID4()
{
	static float speed;
	speed = RM3510_Velocity_Control_motor4(motor204_current_velocity,motor204_target_velocity);
	motor204_current_velocity = (speed * RM3510_x1 + RM3510_Electric_Control_motor4(motor204_current_electric,speed) * RM3510_x2);//加了负号
	motor204_current_electric=speed;
	if(motor204_current_velocity > tyre_ESC_MAX)
	motor204_current_velocity = tyre_ESC_MAX;
	if(motor204_current_velocity < -tyre_ESC_MAX)
	motor204_current_velocity = -tyre_ESC_MAX;		
}


/********************************************************************************
                         820R电调板的电流环控制 
                    输入 820R当前电流  820R目标电流
*********************************************************************************/

float RM3510_Electric_Control_motor1(float current_position_motor,float target_position_motor)
{
    
 
    static float rm_error_l[2] = {0.0,0.0};
    static float rm_output = 0;
    static float rm_inte = 0; 
    
    rm_error_l[0] = rm_error_l[1];
    rm_error_l[1] = target_position_motor - current_position_motor;
    rm_inte += rm_error_l[1]; 
    
    rm_output = rm_error_l[1] * RM3510_ep 
            + rm_inte * RM3510_ei 
            + (rm_error_l[1] - rm_error_l[0]) * RM3510_ed;
    
    if(rm_output > tyre_ESC_MAX)
    {
        rm_output = tyre_ESC_MAX;
    }
    
    if(rm_output < -tyre_ESC_MAX)
    {
        rm_output = -tyre_ESC_MAX;
    }
    		
    return rm_output;
}

float RM3510_Electric_Control_motor2(float current_position_motor,float target_position_motor)
{
    
   
     static float rm_error_l[2] = {0.0,0.0};
    static float rm_output = 0;
    static float rm_inte = 0; 
    
    rm_error_l[0] = rm_error_l[1];
    rm_error_l[1] = target_position_motor - current_position_motor;
    rm_inte += rm_error_l[1]; 
    
    rm_output = rm_error_l[1] * RM3510_ep 
            + rm_inte * RM3510_ei 
            + (rm_error_l[1] - rm_error_l[0]) * RM3510_ed;
    
    if(rm_output > tyre_ESC_MAX)
    {
        rm_output = tyre_ESC_MAX;
    }
    
    if(rm_output < -tyre_ESC_MAX)
    {
        rm_output = -tyre_ESC_MAX;
    }
    		
    return rm_output;
}
float RM3510_Electric_Control_motor3(float current_position_motor,float target_position_motor)
{
   static float rm_error_l[2] = {0.0,0.0};
    static float rm_output = 0;
    static float rm_inte = 0; 
    
    rm_error_l[0] = rm_error_l[1];
    rm_error_l[1] = target_position_motor - current_position_motor;
    rm_inte += rm_error_l[1]; 
    
    rm_output = rm_error_l[1] * RM3510_ep 
            + rm_inte * RM3510_ei 
            + (rm_error_l[1] - rm_error_l[0]) * RM3510_ed;
    
    if(rm_output > tyre_ESC_MAX)
    {
        rm_output = tyre_ESC_MAX;
    }
    
    if(rm_output < -tyre_ESC_MAX)
    {
        rm_output = -tyre_ESC_MAX;
    }
    		
    return rm_output;
}

float RM3510_Electric_Control_motor4(float current_position_motor,float target_position_motor)
{
    static float rm_error_l[2] = {0.0,0.0};
    static float rm_output = 0;
    static float rm_inte = 0; 
    
    rm_error_l[0] = rm_error_l[1];
    rm_error_l[1] = target_position_motor - current_position_motor;
    rm_inte += rm_error_l[1]; 
    
    rm_output = rm_error_l[1] * RM3510_ep 
            + rm_inte * RM3510_ei 
            + (rm_error_l[1] - rm_error_l[0]) * RM3510_ed;
    
    if(rm_output > tyre_ESC_MAX)
    {
        rm_output = tyre_ESC_MAX;
    }
    
    if(rm_output < -tyre_ESC_MAX)
    {
        rm_output = -tyre_ESC_MAX;
    }
    		
    return rm_output;
}


/********************************************************************************
                         820R电调板的速度环控制
                    输入 820R当前速度 820R目标速度
*********************************************************************************/

float RM3510_Velocity_Control_motor1(float current_velocity_motor,float target_velocity_motor)
{
    //const float v_p = 1.0;
	  //const float v_i = 0;
    //const float v_d = 0;
    
   static float rm_error_v[2] = {0.0,0.0};
    static float rm_output = 0;
    
    if(abs(current_velocity_motor) < GAP)
    {
        current_velocity_motor = 0.0;
    }
    
    rm_error_v[0] = rm_error_v[1];
    rm_error_v[1] = target_velocity_motor - current_velocity_motor;
    
    rm_output = rm_error_v[1] * RM3510_vp             
             + (rm_error_v[1] - rm_error_v[0]) * RM3510_vi;
     
    if(rm_output > tyre_ESC_MAX)
    {
        rm_output = tyre_ESC_MAX;
    }
    
    if(rm_output < -tyre_ESC_MAX)
    {
        rm_output = -tyre_ESC_MAX;
    }
		
    return rm_output;//cyq:for6015 反向
}
float RM3510_Velocity_Control_motor2(float current_velocity_motor,float target_velocity_motor)
{
   static float rm_error_v[2] = {0.0,0.0};
    static float rm_output = 0;
    
    if(abs(current_velocity_motor) < GAP)
    {
        current_velocity_motor = 0.0;
    }
    
    rm_error_v[0] = rm_error_v[1];
    rm_error_v[1] = target_velocity_motor - current_velocity_motor;
    
    rm_output = rm_error_v[1] * RM3510_vp             
             + (rm_error_v[1] - rm_error_v[0]) * RM3510_vi;
     
    if(rm_output > tyre_ESC_MAX)
    {
        rm_output = tyre_ESC_MAX;
    }
    
    if(rm_output < -tyre_ESC_MAX)
    {
        rm_output = -tyre_ESC_MAX;
    }
		
    return rm_output;//cyq:for6015 反向
}
float RM3510_Velocity_Control_motor3(float current_velocity_motor,float target_velocity_motor)
{
  static float rm_error_v[2] = {0.0,0.0};
    static float rm_output = 0;
    
    if(abs(current_velocity_motor) < GAP)
    {
        current_velocity_motor = 0.0;
    }
    
    rm_error_v[0] = rm_error_v[1];
    rm_error_v[1] = target_velocity_motor - current_velocity_motor;
    
    rm_output = rm_error_v[1] * RM3510_vp             
             + (rm_error_v[1] - rm_error_v[0]) * RM3510_vi;
     
    if(rm_output > tyre_ESC_MAX)
    {
        rm_output = tyre_ESC_MAX;
    }
    
    if(rm_output < -tyre_ESC_MAX)
    {
        rm_output = -tyre_ESC_MAX;
    }
		
    return rm_output;//cyq:for6015 反向
}
float RM3510_Velocity_Control_motor4(float current_velocity_motor,float target_velocity_motor)
{
    static float rm_error_v[2] = {0.0,0.0};
    static float rm_output = 0;
    
    if(abs(current_velocity_motor) < GAP)
    {
        current_velocity_motor = 0.0;
    }
    
    rm_error_v[0] = rm_error_v[1];
    rm_error_v[1] = target_velocity_motor - current_velocity_motor;
    
    rm_output = rm_error_v[1] * RM3510_vp             
             + (rm_error_v[1] - rm_error_v[0]) * RM3510_vi;
     
    if(rm_output > tyre_ESC_MAX)
    {
        rm_output = tyre_ESC_MAX;
    }
    
    if(rm_output < -tyre_ESC_MAX)
    {
        rm_output = -tyre_ESC_MAX;
    }
		
    return rm_output;//cyq:for6015 反向
}



