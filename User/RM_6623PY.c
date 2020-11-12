#include "main.h" 
float Pitch_speed;
float py_ESC_MAX=5000.0;
float py_INTE_MAX=500.0;
//****************存放所有需要修改的数据*************************
u8 first = 0;          //云台开关，1开启，0关闭
u8 error_gy901 = 0;   //gy901是否出现问题 

/************************陀螺仪数据*************************/
float target = -6.5;  //实现自稳的目标角度 
s16 pitch_current_angle,pitch_target_angle=5200,pitch_target_velocity,pitch_current;//当前  //目标  //驱动
s16 yaw_current_angle,  yaw_target_angle  =5000,yaw_target_velocity,  yaw_current;
s16 pitch_current_velocity;
s16 yaw_current_velocity;

float gy_ap = 20.5;		//38.3;
float gy_ai = 0;
float gy_ad = 40;
float gy_vp = 7.2;		//0.2;
float gy_vi = 0.08;
float gy_vd = 15;
float gy_x1 = 0.5;
float gy_x2 = 1;

float RM6623_pitch_ap = 5.3;
float RM6623_pitch_ai = 0.02;
float RM6623_pitch_vp = 3.2;
float RM6623_pitch_vi = 0.01;
float RM6623_pitch_vd = 8;
float RM6623_pitch_x1 = 0.5;
float RM6623_pitch_x2 = 1.0;

float RM6623_yaw_ap = 5.1;
float RM6623_yaw_ai = 0.01;
float RM6623_yaw_vp = 3.5;
float RM6623_yaw_vi = 0.01;
float RM6623_yaw_vd = 40;
float RM6623_yaw_x1 = 0.5;
float RM6623_yaw_x2 = 1.0;

void APP_Pitch_PID()
{
	static float Pitch_speed;
	Pitch_speed = Position_Control_Pitch(pitch_current_angle,pitch_target_angle);
	pitch_current = Pitch_speed * RM6623_pitch_x1 + 
	Velocity_Control_Pitch(pitch_target_angle - pitch_current_angle,Pitch_speed) * RM6623_pitch_x2;
	if(pitch_current > py_ESC_MAX)
	pitch_current = py_ESC_MAX;
	if(pitch_current < -py_ESC_MAX)
	pitch_current = -py_ESC_MAX;		
}

/********************************************************************************
                         Yaw轴电调板总PID输出
*********************************************************************************/
void APP_Yaw_PID()
{
	static float Yaw_speed;
	Yaw_speed = Position_Control_Yaw(yaw_current_angle,yaw_target_angle);
	yaw_current = Yaw_speed * RM6623_yaw_x1 + 
	Velocity_Control_Yaw(yaw_target_angle - yaw_current_angle,Yaw_speed) * RM6623_yaw_x2;
	if(yaw_current > py_ESC_MAX)
	yaw_current = py_ESC_MAX;
	if(yaw_current < -py_ESC_MAX)
	yaw_current = -py_ESC_MAX;		
}
/********************************************************************************
                           yaw轴电调板的速度环控制
                      输入 yaw轴当前速度 yaw轴目标速度
*********************************************************************************/

float Velocity_Control_Yaw(float current_velocity_yaw,float target_velocity_yaw)
{
    const float v_p = 3.3;
	  const float v_i = 0.01;
    const float v_d = 40.0001;
    
    static float error_v[2] = {0.0,0.0};
    static float output = 0;
		static float inte = 0.0; 
		
    if(abs(current_velocity_yaw) < GAP)
    {
      current_velocity_yaw = 0.0;
    }
    
    error_v[0] = error_v[1];
    error_v[1] = target_velocity_yaw - current_velocity_yaw;
    inte += error_v[1]; 
		
		if(inte>py_INTE_MAX)
		{
		  inte=py_INTE_MAX;
		}
		if(inte<-py_INTE_MAX)
		{
			inte=-py_INTE_MAX; 
		}
		
    output = error_v[1] * v_p
             + (error_v[1] - error_v[0]) * v_d+v_i*inte;
     
    if(output > py_ESC_MAX)
    {
      output = py_ESC_MAX;
    }
    
    if(output < -py_ESC_MAX)
    {
      output = -py_ESC_MAX;
    }
    
    return output;//cyq:for6015 反向
}

/********************************************************************************
                           yaw轴电调板的位置环控制
                      输入 yaw轴当前位置 yaw轴目标位置
*********************************************************************************/
float Position_Control_Yaw(float current_position_yaw,float target_position_yaw)
{
    const float l_p = 5.5;//3#5#:0.760
		const float l_i = 0.000001;//0.000035;
    const float l_d = 0.00001;//3.5;
    
    static float error_l[2] = {0.0,0.0};
    static float output = 0.0;
    static float inte = 0.0;
    
    error_l[0] = error_l[1];   
    error_l[1] = target_position_yaw - current_position_yaw;
    inte += error_l[1]; 

    if(inte>py_INTE_MAX)
		{
			inte=py_INTE_MAX;
		}
		if(inte<-py_INTE_MAX)
		{
			inte=-py_INTE_MAX; 
		}
		
    output = error_l[1] * l_p 
							+ inte * l_i 
							+ (error_l[1] - error_l[0]) * l_d;
    
    if(output > py_ESC_MAX)
    {
        output = py_ESC_MAX;
    }
    
    if(output < -py_ESC_MAX)
    {
        output = -py_ESC_MAX;
    }
    
    return output;
}




/********************************************************************************
                         pitch轴电调板的速度环控制
                    输入 pitch轴当前速度 pitch轴目标速度
*********************************************************************************/
float Velocity_Control_Pitch(float current_velocity_201,float target_velocity_201)
{

	static float error_l[2] = {0.0,0.0};
	static float output = 0.0;
	static float inte = 0.0;

	error_l[0] = error_l[1];
	error_l[1] = target_velocity_201 - current_velocity_201;
	inte += error_l[1];

	if(inte > py_INTE_MAX)
	inte = py_INTE_MAX;
	if(inte < -py_INTE_MAX)
	inte = -py_INTE_MAX;

	output = RM6623_pitch_vp * error_l[1] + RM6623_pitch_vd * (error_l[0] - error_l[1]) + RM6623_pitch_vi * inte;

	if(output > py_ESC_MAX)
	output = py_ESC_MAX;
	if(output < -py_ESC_MAX)
	output = -py_ESC_MAX;

	return output;
}


/********************************************************************************
                         pitch轴电调板的位置环控制 
                    输入 pitch轴当前位置 pitch轴目标位置
*********************************************************************************/
float Position_Control_Pitch(float current_position_201,float target_position_201)
{

	static float error_l = 0.0;
	static float output = 0.0;
	static float inte = 0.0;

	error_l = target_position_201 - current_position_201;
	inte += error_l;

	if(inte > py_INTE_MAX)
	inte = py_INTE_MAX;
	if(inte < -py_INTE_MAX)
	inte = -py_INTE_MAX;

	output = RM6623_pitch_ap * error_l +RM6623_pitch_ai * inte;

	if(output > py_ESC_MAX)
	output = py_ESC_MAX;
	if(output < -py_ESC_MAX)
	output = -py_ESC_MAX;

	return output;
}
/********************************************************************************
                         pitch轴陀螺仪的位置环控制 
                    输入 gy901的 pitch轴当前位置 pitch轴目标位置
*********************************************************************************/
float Angle_PID(float Current_angle,float Goal_angle)
{
	static float error_l[2] = {0.0,0.0};
	static float output = 0.0;
	static float inte = 0.0;

	error_l[0] = error_l[1];
	error_l[1] = Goal_angle - Current_angle;
	inte += error_l[1];

	if(inte > py_INTE_MAX)
	inte = py_INTE_MAX;
	if(inte < -py_INTE_MAX)
	inte = -py_INTE_MAX;

	output = gy_ap * error_l[1] + gy_ai * inte+gy_ad * (error_l[0] - error_l[1]);

	if(output > py_ESC_MAX)
	output = py_ESC_MAX;
	if(output < -py_ESC_MAX)
	output = -py_ESC_MAX;

	return output;
}

/********************************************************************************
                         pitch轴陀螺仪的速度环控制
                    输入 gy901的 pitch轴当前速度 pitch轴目标速度
*********************************************************************************/
float Attitude_RatePID(float Current_rate,float Goal_rate)
{
	static float error_l[2] = {0.0,0.0};
	static float output = 0.0;
	static float inte = 0.0;

	error_l[0] = error_l[1];
	error_l[1] = Goal_rate - Current_rate;
	inte += error_l[1];

	if(inte > py_INTE_MAX)
	inte = py_INTE_MAX;
	if(inte < -py_INTE_MAX)
	inte = -py_INTE_MAX;

	output = gy_vp * error_l[1] + gy_vd * (error_l[0] - error_l[1]) + gy_vi * inte;

	if(output > py_ESC_MAX)
	output = py_ESC_MAX;
	if(output < -py_ESC_MAX)
	output = -py_ESC_MAX;
	return output;
}

/********************************************************************************
                         自稳   pitch轴陀螺仪总PID输出
*********************************************************************************/

void Pitch_PID()
{
	//static float Pitch_speed;
	Pitch_speed = Angle_PID(angle_pitch,target);
	pitch_current = (Pitch_speed * gy_x1 + Attitude_RatePID(-w_y,Pitch_speed) * gy_x2);//加了负号
	if(pitch_current > py_ESC_MAX)
	pitch_current = py_ESC_MAX;
	if(pitch_current < -py_ESC_MAX)
	pitch_current = -py_ESC_MAX;	
}